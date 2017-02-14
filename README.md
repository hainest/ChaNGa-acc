# ChaNGa-acc
Python interface to read ChaNGa binary acceleration files.

---

####Instructions for building
	The standard autotools are used for setup.
	
	./configure
	make
	
	Useful configure options:
	--with-structures-inc=PATH   ChaNGa structures include directory
	--with-xdr-inc=PATH          XDR include directory

	Note: If you are building directly from the git repository, you will need to run `autoconf` to generate the configure file.
---

####Usage
	See test.py for example usage.