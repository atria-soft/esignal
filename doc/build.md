Build lib & build sample                           {#esignal_build}
========================

@tableofcontents

Download:                                          {#esignal_build_download}
=========

esignal use some tools to manage source and build it:

lutin (build-system):                              {#esignal_build_download_lutin}
---------------------

```{.sh}
	pip install lutin --user
	# optionnal dependency of lutin (manage image changing size for application release
	pip install pillow --user
```


dependency:                                        {#esignal_build_download_dependency}
-----------

```{.sh}
	mkdir framework
	cd framework
	git clone https://github.com/atria-soft/elog.git
	git clone https://github.com/atria-soft/etk.git
	git clone https://github.com/atria-soft/ememory.git
	cd ..
```

sources:                                           {#esignal_build_download_sources}
--------

```{.sh}
	cd framework
	git clone https://github.com/atria-soft/esignal.git
	cd ..
```

Build:                                             {#esignal_build_build}
======


library:                                           {#esignal_build_build_library}
--------

```{.sh}
	lutin -mdebug esignal
```

Sample:                                            {#esignal_build_build_sample}
-------

```{.sh}
	lutin -mdebug esignal-sample
	lutin -mdebug esignal-test
```

Or simple:

```{.sh}
	lutin -mdebug esignal-*
```

Run sample:                                        {#esignal_build_run_sample}
===========

```{.sh}
	lutin -mdebug esignal-sample?run
```
