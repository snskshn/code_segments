# .bashrc
# Source global definitions
if [ -f /etc/bashrc ]; then
        . /etc/bashrc
fi
# User specific aliases and functions
#Alias
alias cp='cp -i'
alias mv='mv -i'
alias rm='rm -i'
alias grep='grep -n --color=auto'
alias cls='clear'
alias ls='ls -aCF --color=tty'
alias l='ls -laF'
alias ll='ls -al'
alias ldir='\ls -alF | grep ^d'
alias less4='less -x4'
alias info='info --vi-keys'
alias finds='find . -name "*.[chsS]" -print | xargs grep -n'
alias vi=vim
#FOS20 CVS environment value export
#export CVSROOT=:ext:ssson@10.25.1.20:/future/cvsuser/cvsdir
export CVSROOT=:ext:ssson@10.25.1.20:/work/subversion
export EDITOR=vim
export VISUAL=vim
export CVS_EDITOR=vim
export CVS_RSH=ssh
export SVN=svn+ssh://10.25.1.20/work/subversion
export SVN_EDITOR=vim
# User Script
alias fa='find . -name "*" |xargs grep -n -w'
alias fac='find . -name "*.c" |xargs grep -n -w'
alias fah='find . -name "*.h" |xargs grep -n -w'
alias fach='find . -name "*.[c,h]" |xargs grep -n -w'

clear
