pySSDeep
========

Python SSDeep  wrapper

Requirements
============

SSDeep library/tool : http://ssdeep.sourceforge.net

Installation
============

Download or clone the source

$ python setup.py build
$ sudo python setup.py install

Example
=======
```
>>> import pyssdeep
>>> sig1 = pyssdeep.fuzzy_hash_filename("ls_test1")
>>> sig2 = pyssdeep.fuzzy_hash_buf(open("ls_test2").read())
>>> pyssdeep.fuzzy_compare(sig1, sig2)
99
>>>
``` 
