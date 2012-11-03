# Bootstrap the build
topdir() {
    (cd "$(pwd)/$(dirname $0)"; pwd)
}
ROVERS=${ROVERS:-`topdir`}
BUILDDIR=${ROVERS}/build
[ -d ${BUILDDIR} ] || mkdir -p ${BUILDDIR}

# Dependencies
[ -d ${BUILDDIR}/depends ] || mkdir -p ${BUILDDIR}/depends
(cd ${BUILDDIR}/depends; 
 cmake ${ROVERS}/depends -DROVERS_BINARY_DIR=${BUILDDIR} -G "Unix Makefiles" &&
 make)

# Main
(cd ${BUILDDIR}; cmake -G "Unix Makefiles"  ${ROVERS}/src -DGTEST_ROOT=${BUILDDIR} -DCMAKE_BUILD_TYPE:STRING=Debug)
echo "(cd ${BUILDDIR}; make)"

