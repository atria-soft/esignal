# esignal - Signal/Slot Event System

## Architecture

Signal/slot system using weak references for automatic lifecycle management. Signals hold a list of `ConnectedElement*` variants, each wrapping the callback and tracking object/connection lifetimes via `WeakReference`.

## Connection API — Which method to use

### `connectAuto(object, function)` — Preferred for object-scoped connections
```java
// Connection lives as long as `this` is alive. No return value to store.
signal.connectAuto(this, (self, value) -> self.onValueChanged(value));
// or with method reference (function signature must accept object as first param):
signal.connectAuto(this, MyClass::onValueChanged);
```
- Object stored as `WeakReference` — auto-cleanup when object is GC'd
- Function stored as strong reference
- **No `Connection` returned** — no need to store anything
- Best for: connecting in constructors, lifecycle-scoped listeners

### `connectAutoRemoveObject(object, function)` — Same but with direct callback
```java
// Same auto-cleanup, but callback doesn't receive the object as parameter
signal.connectAutoRemoveObject(this, () -> doSomething());
```
- Same weak-reference-on-object behavior as `connectAuto`
- Callback signature matches the signal type directly (no object parameter)

### `connect(function)` — Returns Connection, must store it
```java
// MUST store the Connection — it is held by WeakReference internally!
this.connection = signal.connect(() -> doSomething());
```
- Connection stored as `WeakReference` — if you don't keep a strong reference, the connection will be silently removed at next `emit()`
- Function stored as strong reference
- Best for: connections you need to explicitly `close()` later

### `connect(object, function)` — Returns Connection, must store it
```java
// MUST store the Connection — BOTH object and connection are WeakReferences!
this.connection = signal.connect(this, MyClass::onValueChanged);
```
- Object stored as `WeakReference`
- Connection stored as `WeakReference`
- `lockObjects()` checks BOTH — if either is GC'd, connection is auto-removed
- Best for: connections you need to explicitly `close()` later AND want auto-cleanup on object death

### `connectWeak(function)` — Weak reference on callback itself
```java
// Must keep a strong reference to the function object itself
this.handler = this::onEvent;
signal.connectWeak(this.handler);
```
- Function stored as `WeakReference` — GC'd if you don't keep a reference
- No `Connection` returned
- Rarely used directly

## Critical Rule: `connect()` return value

**All `connect()` methods that return a `Connection` are annotated `@CheckReturnValue`.**

If you call `connect()` or `connect(object, function)` without storing the returned `Connection`, the connection WILL be silently garbage collected and removed. This is by design — it is NOT a bug.

### Common mistake (CONNECTION SILENTLY LOST):
```java
// BAD: Connection is not stored, eligible for GC immediately
signal.connect(this, MyClass::onEvent);  // return value discarded!
signal.connect(() -> doSomething());     // return value discarded!
```

### Correct patterns:
```java
// GOOD: Store in a field
this.myConnection = signal.connect(this, MyClass::onEvent);

// GOOD: Store in a list (for bulk cleanup)
this.connections.add(signal.connect(() -> doSomething()));

// GOOD: Use connectAuto instead (no Connection to store)
signal.connectAuto(this, MyClass::onEvent);
```

### There is NO problem with `this` in constructors
The weak reference on `this` in the constructor is safe because the object being constructed is strongly reachable from the calling code (it's being assigned to a variable or field). The `this` reference is valid throughout the constructor. The only risk is not storing the returned `Connection`.

## Cleanup Patterns

```java
// Single connection cleanup
this.connection.close();

// Bulk cleanup (clear a list of connections)
this.connections.forEach(Connection::close);
this.connections.clear();

// Or just let GC handle it (connections auto-removed when Connection or object is GC'd)
```
