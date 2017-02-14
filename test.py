import ChaNGa
import numpy as np
import os

filename = 'tipsy.test'
num_particles = 1123

for type in [np.float32, np.float64]:
    acc = np.require(np.arange(num_particles*3, dtype=type).reshape(num_particles, 3),
                     dtype=type,
                     requirements=['C_CONTIGUOUS', 'ALIGNED', 'WRITEABLE', 'OWNDATA', 'ENSUREARRAY'])
    
    with ChaNGa.File(filename) as f:
        f.acc = acc
    
    with ChaNGa.File(filename) as f:
        read_acc = f.acc
    
    if np.all(np.abs(acc - read_acc) <= 1e-12):
        print("{0} Passed".format(type))
    else:
        print('{0} Failed'.format(type))
    
    os.unlink(filename)
