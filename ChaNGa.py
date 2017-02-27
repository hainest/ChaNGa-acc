import numpy as np
import ctypes
import numpy.ctypeslib as npct

class File:
    def __init__(self, filename):
        self.lib = _load_lib()
        # This print is needed to bypass a bug in the Python ctypes system
        print()
        self.cfname = ctypes.c_char_p(bytes(filename, 'utf-8'))
        self.data = None
        self.num_particles = ctypes.c_size_t(0)
        self.is_double = ctypes.c_int(0)

    @property
    def acc(self):
        if self.data is None:
            self.lib.ChaNGa_py_read_header(self.cfname, ctypes.byref(self.num_particles),
                                           ctypes.byref(self.is_double))
            if int(self.is_double.value) == 1:
                self.data = np.empty((self.num_particles.value, 3), dtype=np.float64)
                self.lib.ChaNGa_py_read_accelerations_double(self.cfname, self.data, self.num_particles)
            else:
                self.data = np.empty((self.num_particles.value, 3), dtype=np.float32)
                self.lib.ChaNGa_py_read_accelerations_float(self.cfname, self.data, self.num_particles)
        return self.data
    
    @acc.setter
    def acc(self, acc):
        self.data = acc
        self.num_particles = ctypes.c_size_t(int(acc.size / 3))
        self.is_double = False
        if acc.dtype == np.float32:
            self.lib.ChaNGa_py_write_accelerations_float(self.cfname, self.data, self.num_particles)
        elif acc.dtype == np.float64:
            self.is_double = True
            self.lib.ChaNGa_py_write_accelerations_double(self.cfname, self.data, self.num_particles)
        else:
            raise TypeError('acc must be np.float32 or np.float64')

    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc_value, traceback):
        return False  # always re-raise exceptions

def _load_lib():
    """For internal use only """
    if _load_lib.lib is not None:
        return _load_lib.lib
    
    lib = npct.load_library("libchangaacc", "")
    
    def decode_err(err):
        if int(err) != 0:
            raise IOError(lib.ChaNGa_py_strerror(err).decode('utf-8'))
    
    _array_2d_float32 = npct.ndpointer(dtype=np.float32, ndim=2, flags=('C','O','W','A'))
    _array_2d_float64 = npct.ndpointer(dtype=np.float64, ndim=2, flags=('C','O','W','A'))
    
    # Force parameter type-checking and return-value error checking
    lib.ChaNGa_py_strerror.restype = ctypes.c_char_p
    lib.ChaNGa_py_strerror.argtypes = [ctypes.c_int]
    
    lib.ChaNGa_py_read_header.restype = decode_err
    lib.ChaNGa_py_read_header.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_size_t), ctypes.POINTER(ctypes.c_int)]

    lib.ChaNGa_py_read_accelerations_double.restype = decode_err
    lib.ChaNGa_py_read_accelerations_double.argtypes = [ctypes.c_char_p, _array_2d_float64, ctypes.c_size_t]

    lib.ChaNGa_py_write_accelerations_double.restype = decode_err
    lib.ChaNGa_py_write_accelerations_double.argtypes = [ctypes.c_char_p, _array_2d_float64, ctypes.c_size_t]
    
    lib.ChaNGa_py_read_accelerations_float.restype = decode_err
    lib.ChaNGa_py_read_accelerations_float.argtypes = [ctypes.c_char_p, _array_2d_float32, ctypes.c_size_t]

    lib.ChaNGa_py_write_accelerations_float.restype = decode_err
    lib.ChaNGa_py_write_accelerations_float.argtypes = [ctypes.c_char_p, _array_2d_float32, ctypes.c_size_t]

    _load_lib.lib = lib
    return lib
_load_lib.lib = None

