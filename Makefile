SHELL=/bin/bash -e
all: clean build

build:
	CFLAGS="-g3 -O2" meson setup build $(ARGS) \
	    --buildtype=debug \
	    -Dplugindir=`pwd`/build \
	    -Ddebug=true \
	    && \
	ninja -C build -v

clean:
	rm -rvf build
