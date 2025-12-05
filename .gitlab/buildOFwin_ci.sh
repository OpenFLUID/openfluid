# WARNING, check if OF correctly uninstalled before operating
echo "Lancement du script..."
OPENFLUID_REPO=$PWD
BUILD_DIR=$OPENFLUID_REPO/_build
mkdir -p $BUILD_DIR

cd $BUILD_DIR
export PATH=/mingw64/bin:$PATH
export OPENFLUID_INSTALL_PREFIX=`pwd`/dist
export OFBUILD_SUPPORT_DIR=/mingw64/
export PATH=$OPENFLUID_INSTALL_PREFIX/bin:$OPENFLUID_INSTALL_PREFIX/lib:$PATH

cmake $OPENFLUID_REPO -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug -DOFBUILD_SUPPORT_DIR=/mingw64  -DCMAKE_INSTALL_PREFIX=. -DOFBUILD_ENABLE_DOCS=OFF -DOFBUILD_ENABLE_GUI=OFF
make -j 30 > $BUILD_DIR/build_out.log