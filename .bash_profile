# .bash_profile

# Get the aliases and functions
if [ -f ~/.bashrc ]; then
	. ~/.bashrc
fi

# User specific environment and startup programs

PATH=$PATH:$HOME/bin
#PATH=/usr/local/bin2:$PATH:$HOME/bin

PATH=/usr/local/Cavium_Networks/OCTEON-SDK/tools-gcc-4.1/bin:$PATH:/pub/fp_be/bin:/usr/lib/ccache


export VISUAL=vi
export EDITOR=vi
export SVN=svn+ssh://10.25.1.20/work/subversion

export PATH
pushd /usr/local/Cavium_Networks/OCTEON-SDK
source env-setup OCTEON_CN3010
popd

linux32 bash
