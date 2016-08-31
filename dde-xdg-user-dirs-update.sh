#!/bin/bash

DIRS=(DESKTOP DOWNLOAD TEMPLATES PUBLICSHARE DOCUMENTS PICTURES MUSIC VIDEOS)
dirnames=(Desktop Downloads Templates Public Documents Pictures Music videos)

i=0
for item in "${DIRS[@]}"; do
    dir=$(xdg-user-dir $item)
    # echo $dir
    # echo ~/${dirnames[i]}
    if [[ $dir == $HOME/ ]]; then
        xdg-user-dirs-update --set $item  ~/${dirnames[i]}
        mkdir -p ~/${dirnames[i]}
    fi
    i=$i+1
done
