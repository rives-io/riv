#!/bin/sh
if [ -z "$GID" -o -z "$UID" -o -z "$USER" -o -z "$GROUP" ]; then
  echo Running as $(whoami)
  exec "$@"
else
  if [ ! $(getent group $GID) ]; then
    if [ $(getent group $GROUP) ]; then
      echo Group name $GROUP already exists
      GROUP=container-group-$GID
    fi
    addgroup -g $GID $GROUP
  else
    echo The id $GID of group $GROUP already exists
  fi
  if [ ! $(getent passwd $UID) ]; then
    if [ $(getent passwd $USER) ]; then
      echo User name $USER already exists.
      USER=container-user-$UID
    fi
    adduser -D -u $UID -G $GROUP $USER
  else
    echo The id $UID of user $USER already exists
  fi
  USERNAME=$(getent passwd "$UID" | cut -d: -f1)
  export HOME=/home/$USERNAME
  mkdir -p $HOME
  chown $UID:$GID $HOME
  if [ -t 0 ]; then
    chown $UID:$GID $(/usr/bin/tty)
  fi
  echo Running as $USERNAME
  exec su -p $USERNAME -c "$@"
fi
