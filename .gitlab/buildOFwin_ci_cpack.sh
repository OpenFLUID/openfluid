# WARNING, check if OF correctly uninstalled before operating
echo "Lancement du script..."
OPENFLUID_REPO=$PWD
sed -i "s/#INSTALL(DIRECTORY/INSTALL(DIRECTORY/" CPack.win32.in.cmake #activate local windows line

BUILD_DIR=$OPENFLUID_REPO/_build_install
mkdir -p $BUILD_DIR

cd $BUILD_DIR
export PATH=/mingw64/bin:$PATH
export OPENFLUID_INSTALL_PREFIX=`pwd`/dist
export OFBUILD_SUPPORT_DIR=/mingw64/
export PATH=$OPENFLUID_INSTALL_PREFIX/bin:$OPENFLUID_INSTALL_PREFIX/lib:$PATH

cmake $OPENFLUID_REPO -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release -DOFBUILD_SUPPORT_DIR=/mingw64  -DCMAKE_INSTALL_PREFIX=. -DOFBUILD_ENABLE_DOCS=OFF
make -j 30 > $BUILD_DIR/make_out.log
cpack -j 30 > $BUILD_DIR/cpack_out.log