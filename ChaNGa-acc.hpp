#pragma once

#include "tree_xdr.h"
#include <errno.h>
#include <rpc/xdr.h>
#include "OrientedBox.h"
#include <stdexcept>
#include <vector>

namespace ChaNGa {

	/*! A drop-in RAII wrapper around a C FILE* */
	class cfile_ptr {
		FILE *fp;
	public:
		explicit cfile_ptr(char const* filename, char const* mode) : fp{std::fopen(filename, mode)} {}
		~cfile_ptr() { if (fp) fclose(fp); }
		explicit operator bool() const { return fp != nullptr; }

		/*
		 * This conversion operator isn't explicit so that no
		 * casts are necessary to use the C stdio functions
		 */
		operator FILE*() const { return fp; }
	};

	struct cio_error : public std::exception {
		int error;
		cio_error(int err) : error{err} {}
	};

namespace detail {
	template <typename T>
	struct xdr_trans {};

	template<> struct xdr_trans<float> { static constexpr auto func = xdr_float; };
	template<> struct xdr_trans<double> { static constexpr auto func = xdr_double; };
}

template <typename T>
void read_accelerations(char const *filename, T* acc, size_t size) {
	cfile_ptr fd{filename, "rb"};
	if (!fd) {
		throw cio_error{errno};
	}
	XDR xdr;
	xdrstdio_create(&xdr, fd, XDR_DECODE);

	FieldHeader fh;
	auto status = xdr_template(&xdr, &fh);

	OrientedBox<double> accelerationBox;
	status &= xdr_template(&xdr, &accelerationBox);

	status &= xdr_vector(&xdr, reinterpret_cast<char *>(acc), static_cast<unsigned>(size),
						 sizeof(T), (xdrproc_t)detail::xdr_trans<T>::func);
	xdr_destroy(&xdr);
	if(status != 1) throw std::runtime_error{"Bad XDR read"};
}

/*! Writer just for testing */
template <typename T>
void write_accelerations(char const *filename, T *acc, size_t size) {
	cfile_ptr fd{filename, "wb"};
	if (errno != 0) {
		throw cio_error{errno};
	}
	XDR xdr;
	xdrstdio_create(&xdr, fd, XDR_ENCODE);
	FieldHeader fh;
	fh.time = 0.0;
	fh.numParticles = size / 3;
	fh.dimensions = 3;
	fh.code = TypeHandling::Type2Code<T>::code;
	auto status = xdr_template(&xdr, &fh);
	OrientedBox<double> accelerationBox;
	status &= xdr_template(&xdr, &accelerationBox);
	status &= xdr_vector(&xdr, reinterpret_cast<char *>(acc), static_cast<unsigned>(size),
						sizeof(T), (xdrproc_t)detail::xdr_trans<T>::func);
	xdr_destroy(&xdr);
	if(status != 1) throw std::runtime_error{"Bad XDR write"};
}

}


