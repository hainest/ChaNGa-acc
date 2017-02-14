#include "ChaNGa-acc.hpp"

#include <cstdio>
#include <cstring>
#include <cerrno>

enum {
	BAD_READ = 0x10004, /* Bad read on XDR stream */
	BAD_WRITE = 0x10005 /* Bad write on XDR stream */
};

extern "C" int ChaNGa_py_read_header(char const *filename, size_t *size, int *is_double) {
	ChaNGa::cfile_ptr fd { filename, "rb" };
	if (!fd) {
		return errno;
	}
	XDR xdr;
	xdrstdio_create(&xdr, fd, XDR_DECODE);

	FieldHeader fh;
	auto status = xdr_template(&xdr, &fh);
	xdr_destroy(&xdr);

	*size = fh.numParticles;
	*is_double = fh.code == TypeHandling::Type2Code<double>::code;

	return status ? 0 : BAD_READ;
}
extern "C" int ChaNGa_py_read_accelerations_double(char const *filename, double (*acc)[3], size_t num_particles) {
	try {
		ChaNGa::read_accelerations(filename, (double *) acc, 3 * num_particles);
	} catch (ChaNGa::cio_error &e) {
		return e.error;
	} catch (...) {
		return BAD_READ;
	}
	return 0;
}
extern "C" int ChaNGa_py_write_accelerations_double(char const *filename, double (*acc)[3], size_t num_particles) {
	try {
		ChaNGa::write_accelerations(filename, (double *) acc, 3 * num_particles);
	} catch (ChaNGa::cio_error &e) {
		return e.error;
	} catch (...) {
		return BAD_WRITE;
	}
	return 0;
}
extern "C" int ChaNGa_py_read_accelerations_float(char const *filename, float (*acc)[3], size_t num_particles) {
	try {
		ChaNGa::read_accelerations(filename, (float *) acc, 3 * num_particles);
	} catch (ChaNGa::cio_error &e) {
		return e.error;
	} catch (...) {
		return BAD_READ;
	}
	return 0;
}
extern "C" int ChaNGa_py_write_accelerations_float(char const *filename, float (*acc)[3], size_t num_particles) {
	try {
		ChaNGa::write_accelerations(filename, (float *) acc, 3 * num_particles);
	} catch (ChaNGa::cio_error &e) {
		return e.error;
	} catch (...) {
		return BAD_WRITE;
	}
	return 0;
}
extern "C" char const *ChaNGa_py_strerror(int err) {
	switch (err) {
	case BAD_READ:
		return "XDR read failed";
	case BAD_WRITE:
		return "XDR write failed";
	default:
		return strerror(err);
	}
}
