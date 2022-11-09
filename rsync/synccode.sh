#!/bin/bash

################################################################
##                 _
##    ___ ___   __| | ___  ___ _   _ _ __   ___
##   / __/ _ \ / _` |/ _ \/ __| | | | '_ \ / __|
##  | (_| (_) | (_| |  __/\__ \ |_| | | | | (__
##   \___\___/ \__,_|\___||___/\__, |_| |_|\___|
##                             |___/
################################################################
# auther: CQzhangyu; yongchao He
# email: 1098643527@qq.com
################################################################

username=zhaochenqi
BE_DIR="/home/zcq"

PUSH_LIST=(
    # "local_dir server_name server_dir"
    "project pku_server /home/zcq"
)
PULL_LIST=(
    # "local_dir server_name server_dir"
    "project pku_server /home/zcq"
)
EXCLUDE="--exclude-from=/home/zcq/utils/exclude.list"
################################################################

color_black="\033[1;30m"
color_red="\033[1;31m"
color_green="\033[1;32m"
color_yellow="\033[1;33m"
color_blue="\033[1;34m"
color_purple="\033[1;35m"
color_skyblue="\033[1;36m"
color_white="\033[1;37m"
color_reset="\033[0m"
echo_back() {
    cmdLog=${1}
    printf "[${color_green}EXEC${color_reset}] ${cmdLog}\n"
    eval ${cmdLog}
}
echo_info() {
    cmdLog=${1}
    printf "[${color_green}INFO${color_reset}] ${cmdLog}\n"
}
echo_warn() {
    cmdLog=${1}
    printf "[${color_yellow}WARN${color_reset}] ${cmdLog}\n"
}
echo_erro() {
    cmdLog=${1}
    printf "[${color_red}ERRO${color_reset}] ${cmdLog}\n"
}

show_usage() {
    appname=$0
    echo_info "Usage: ${appname} [Options]"
    echo_info "  -- -s       start pushing files to remote servers" 
    echo_info "  -- -l       start pulling files from remote servers" 
    echo_info "  -- -c       stop and clean rsync processes"
}

sync_stop() {
    echo_info "Terminate rsync..."
    for pid in `pgrep inotifywait`; do
        echo_back "sudo kill -9 $pid"
    done
    for pid in `pgrep rsync`; do
        echo_back "sudo kill -9 $pid"
    done
}

# Example
# rsync --delete -avzhcPe 'ssh -T -p xx user@ip -t ssh -p yy -i ~/.ssh/id_rsa' local_dir user@ip:remote_dir

push_work() {
    local bedir="${BE_DIR}/$1"
    local redir=$3
    local raddr=$2
    if [ ! ${redir} ]; then
        echo_warn "Please specify the remote directory"
        exit 0
    fi
    local bkdir=${BE_DIR}/rsync_bak/${raddr}
    if [ ! -d ${bkdir} ]; then
        echo_back "mkdir -p ${bkdir}"
    fi
    echo_back "rsync --delete -r ${EXCLUDE} ${raddr}:${redir}/$1 ${bkdir}"

    echo_info "Synchronizing ${bedir} to ${raddr}:${redir}"
    
    echo_back "rsync --delete -r ${EXCLUDE} $bedir ${raddr}:${redir}"
    
    inotifywait -mrq -e close_write,modify,delete,create $bedir | while read f; do rsync --delete -r ${EXCLUDE} $bedir ${raddr}:${redir}; done &
    echo_info "Done! (PID: $!)"
}

pull_work() {
    local bedir=${BE_DIR}
    local redir="$3/$1"
    local raddr=$2
    if [ ! ${redir} ]; then
        echo_warn "Please specify the remote directory"
        exit 0
    fi
    if [ -d ${bedir}/$1 ]; then
        local bkdir=${BE_DIR}/rsync_bak/local
        if [ ! -d ${bkdir} ]; then
            echo_back "mkdir -p ${bkdir}"
        fi
        echo_back "rsync --delete -r ${EXCLUDE} ${bedir}/$1 ${bkdir}"
    fi

    echo_info "Synchronizing ${raddr}:${redir} to ${bedir}"
    echo_back "rsync --delete -r ${EXCLUDE} ${raddr}:${redir} $bedir"
    while :; do
        rsync --delete -r ${EXCLUDE} ${raddr}:${redir} $bedir
        sleep 10
    done
    echo_info "Done."
}

push_start() {
    if [ ! ${BE_DIR} ]; then
        echo_warn "Please specify the local directory"
        exit 0
    fi
    if [ ! -d ${BE_DIR} ]; then
        echo_erro "${BE_DIR} does not exist"
        exit 0
    fi
    for ssh_item in "${PUSH_LIST[@]}"; do
        local ssh_para=(${ssh_item})
        if (( ${#ssh_para[*]} == 3 )); then
            push_work ${ssh_para[0]} ${ssh_para[1]} ${ssh_para[2]}
        else
            echo_erro "unsupported format: ${ssh_para}"
        fi
    done
}

pull_start() {
    if [ ! ${BE_DIR} ]; then
        echo_warn "Please specify the local directory"
        exit 0
    fi
    if [ ! -d ${BE_DIR} ]; then
        echo_erro "${BE_DIR} does not exist"
        exit 0
    fi
    for ssh_item in "${PULL_LIST[@]}"; do
        local ssh_para=(${ssh_item})
        if (( ${#ssh_para[*]} == 3 )); then
            pull_work ${ssh_para[0]} ${ssh_para[1]} ${ssh_para[2]}
        else
            echo_erro "unsupported format: ${ssh_para}"
        fi
    done
}

para_checker() {
    if [ ! ${username} ]; then
        echo_erro "Please provide a valid username" 
        exit 0
    fi
    if [ ! ${BE_DIR} ]; then
        echo_erro "Please provide a valid directory for BE_DIR" 
        exit 0
    fi
}

################################################################
####################    * Main Process *    ####################
################################################################
if (( $# == 0 )); then
    show_usage
    exit 0
fi

if (( $UID == 0 )); then
    echo_erro "Don't run this script as root"
    exit 0
fi

para_checker

while getopts "cmsldh" opt;
do
    case $opt in
        c)
            sync_stop
            ;;
        s)
            push_start
            ;;
        l)
            pull_start
            ;;
        h)
            show_usage
            ;;
        \?)
            show_usage
            ;;
    esac
done
