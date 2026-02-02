## general info

#### resources

https://wiki.archlinux.org/title/Cron

You can also use systemd as a replacement for cron:\
https://wiki.archlinux.org/title/Systemd/Timers

## syntax

#### leading whitespace

Any amount of whitespace is allowed in front of a crontab entry.

## installation

#### fedora

Unter `Fedora` muss erst das Paket `cronie` installiert werden.


```
systemctl enable --now crond
```

#### arch

```
sudo pacman -S cronie
sudo systemctl enable --now cronie
```

#### usage

view crontab of the current user:
```
crontab -l
```

edit the crontab of the current user:
```
crontab -e
```

## troubleshooting

### you have to escape %

**escape `%` characters with `\`!**
```
0 20 * * * /home/myuser/scripts/backup.sh > ${HOME}/logs/backup$(date +\%Y\%m\%d).log 2>&1
```
