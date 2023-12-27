# Run . ./setup_env.sh
if [[ ! $PATH = *"qemu"* ]]; then
    export PATH=$PATH:/home/yasaman/edk2/qemu-8.1.2/build
fi

export WORKSPACE=/home/yasaman/edk2
export EDK_TOOLS_PATH=/home/yasaman/edk2/BaseTools
export CONF_PATH=`pwd`/Conf

ln -s `pwd`/AzSakhtPkg $WORKSPACE/AzSakhtPkg
. /home/yasaman/edk2/edksetup.sh