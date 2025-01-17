## alsa

ALSA is part of the kernel.

What is PCM or a PCM device?\
https://en.wikipedia.org/wiki/Pulse-code_modulation

#### aplay

`aplay` is part of the package `alsa-utils` on Fedora.
```
aplay -l
```
List all soundcards and digital audio devices.

```
aplay -L
```
-L, --list-pcms\
List all PCMs defined\
(there is also `default`)

You can use `aplay` to play audio files.\
Without argument it works on wav/raw files:
```
aplay example.wav
```
With `-f` there are different formats available, but you would need to be an audio expert to understand what these mean.

***
speaker-test is another useful binary from `alsa-utils`:
```
speaker-test -l 1
```
This will produce some noise on the default playback device for 10 seconds\
and print some info about it on the screen.\
See `man speaker-test` for more.

Also useful:
```
amixer
``` 

```
amixer info
```
You can also specify the device
```
amixer -D default
```
(and you can also specify the card with `-c`)

```
amixer controls
```

There is also the package `alsa-topology-utils` which provides the command `alsatplg`.

#### configuration

```
$ cat /etc/alsa/alsactl.conf 
#
# ALSA library configuration file for alsactl tool
# (/usr/share/alsa tree is not necessary for alsactl)
#

ctl.hw {
	@args [ CARD ]
	@args.CARD {
		type string
		default "0"
	}
	type hw
	card $CARD
}
```

And there  is stuff here:
```
/etc/alsa/conf.d $ ls
50-pipewire.conf  99-pipewire-default.conf
```

#### select default soundcard and/or pcm device

https://www.alsa-project.org/wiki/Setting_the_default_device

=> that works

Find your desired card with:
```
cat /proc/asound/cards
```

and then create /etc/asound.conf with following:
```
defaults.pcm.card 1
defaults.ctl.card 1
```
Replace "1" with number of your card determined above. 

you can test with `speaker-test -l 1` or with the minimal pcm example code

**Note: The problem is, that your USB headset might be card 0 one time and when you replug it another time it might be card 1.**

***

#### audioservers

`pulseaudio` and `pipewire` are audio servers.\
They are on top of alsa.

## how to figure out if you are using pulseaudio or pipewire?

```
$ ps -ef | grep -i "pipe"
bf          2079    1856  0 19:09 ?        00:00:10 /usr/bin/pipewire
bf          2082    1856  0 19:09 ?        00:00:13 /usr/bin/pipewire-pulse
```

pipewire doesnt seem to have a systemd unit

But there seems to a compatibility layer involved since `pulseaudio volume control` (or pavucontrol) does change my audio output volume.

***

## pipewire

https://docs.pipewire.org/

`/usr/share/pipewire/pipewire.conf`

The program `qpwgraph` is supposed to be a gui help for pipewire.\
Seems very chaotic though.

#### pipewire - record audio

If you use `pipewire` you can test your input device by recording like this:
```
pw-cat -r recordme
```

#### pipe audio to pipewire

```
cat Hitman_Budapest_Bath_Hotel.mp3 | paplay
```

***
## pulseaudio

#### installation

```
sudo pacman -S pulseaudio
sudo pacman -S pavucontrol
sudo reboot
```

#### pulseaudio commands

```
pactl info
```

```
paplay <audiofile>
```

#### testing pulseaudio

```
pactl load-module module-sine frequency=440
```
stop the sound with
```
pactl unload-module module-sine
```

btw, to see all loaded modules:
```
pactl list modules
```

---

#### ffmpeg

Puts the audio of a .mp4 Video into a .mp3 file:
```
ffmpeg -i input.mp4 output.mp3
```
Converting to .webm works the same way:
```
ffmpeg -i input.mp4 output.webm
```
Reducing the resolution of a video:
```
ffmpeg -i <input> -vf scale=480x360,setdar=4:3 <output>
```
---

#### play sounds on the command line

Make sure "system sounds" are almost as high as the other sounds in `pulse audio volume control`.

```
mpg123 -q /path/to/myfile.mp3
```

or directly with pulseaudio:
```
paplay /usr/share/sounds/freedesktop/stereo/alarm-clock-elapsed.oga
```

---

#### test microphone

part of the `libpulse` package:
```
parecord file.mp3
```

```
pacman -F parecord
extra/libpulse 16.1-6 [installed]
    usr/bin/parecord
    usr/share/bash-completion/completions/parecord
```

The `pulseaudio-utils` package has the `parec` command:
```
parec --format=s16le --channels=2 --rate=44100 | pacat
```
This will record from your microphone and output it a couple of seconds later.

## Volume problem in movies

In movies the volume is always inappropriate, shootings and music is very loud,
quiet conversations you want to hear are unhearable silent.
In my experience while using headphones this problem is less dominant but still exists.

***

Using OpenShot:
Imported movie, separate audio command took forever so I quit.

Tried it another time. Waited 2 minutes switched to browser in the meantime.
Then it worked.
But how do I just cut the audio without cutting the video clip too?
=> don't use short cut, use the mouse, left click to select the clip,
right click on the clip -> slice -> keep both sides
Then select the clip, right click and put the volume to 10% or delete the audio clip.

=> this works ok, but the performance is a little slow, maybe reduce the quality of the video
before doing this!

Exporting was a problem at 720p 30fps medium quality.
Took 1 hour and the file is now 4.3GB big which is way too much.
The problem was the original file was 720X320 at 24 fps and the new file is 720x1980 at 30fps.
So I chose the wrong export option.

Rendering/exporting video has to be done in advanced mode.
You need to view the video and audio settings of the original video in file browser (video bitrate,
audio bitrate, sample rate, codec, dimensions).
The file size of the exported video is ok now, but I still have problems with the resolution.
The video is squished together and the black frames on bottom and top are bigger than orignal
even though I selected the same resolution.
Außerdem Audiotrack verschoben...

***

VLC Media Player:

Tools -> Effects and Filters (ctrl + E)

=> try experimenting with enabling equalizer and/or enabling compressor
==> doesn't really seem to help, maybe you have to change some settings
===> changed settings, doesn't help

----------------------------------------------------------------------------------------

Try that didn't get me anywhere:

sudo apt install pulseaudio-equalizer

command to start the program:
qpaeq

Error
solution: https://linuxhint.com/install-pulseaudio-equalizer-linux-mint/

sudo vim /etc/pulse/default.pa

Add the below-given lines at the bottom of the/etc/pulse/default.pa file.
load-module module-equalizer-sink
load-module module-dbus-protocol

Next, kill the PulseAudio service and restart it:
$ pulseaudio --kill && pulseaudio --start

Now, open the PulseAudio with the command:
$ qpaeq

=> Ok, jetzt funktioniert zwar das Programm, aber ich kann mit dem Programm nichts anfangen.
Scheint auch keine Dateien zu manipulieren sondern nur generell den Output.

***

#### audacity

```
sudo apt install audacity
```

Cut out audio: Select a range with the mouse and hit <kbd>del</kbd>

Keep everything but the selected: select a range with the mouse and go to `edit -> remove special -> trim audio`. (or <kbd>ctrl</kbd> + <kbd>t</kbd>\
Then put it on the beginning of the track with `tracks -> align tracks -> start to zero`




#### location of sound files that applications use

chatgpt:\
The location of sound files used by a Linux application can vary depending on the application and the specific Linux distribution being used. However, there are a few common locations where sound files are typically stored:

1. /usr/share/sounds: This directory contains system sounds that are used by many Linux applications, such as notifications and alerts.

1. /usr/share/sounds/{application-name}: Some applications may have their own subdirectory within /usr/share/sounds where they store their sound files.

1. ~/.local/share/sounds: This directory contains sound files for user-specific applications, such as media players or games.

1. /usr/lib/x86_64-linux-gnu/pulseaudio: This directory contains system sounds for the PulseAudio sound server, which is used by many Linux distributions.

1. /usr/lib/x86_64-linux-gnu/gstreamer-1.0: This directory contains sound files used by GStreamer, a popular multimedia framework used by many Linux applications.

For example this file existed both on Arch Linux and on Fedora:
```
$ paplay /usr/share/sounds/freedesktop/stereo/alarm-clock-elapsed.oga
```

## troubleshooting

#### no audio input from headset

I had no audio input from my headset.

The solution was to go into `pulse audio volume control` or `pavucontrol`.\
"Input devices" -> And set your headset as `fallback`

With `pipewire` you can test it with `pw-cat -r myfile`, go into `nnn` and hit enter on the file.

***

#### no sound issue on Ubuntu

I had the issue that no sound was working on Ubuntu anymore because of various system problems.

Then I did

```
sudo apt reinstall alsa-base
```
```
sudo apt reinstall pulseaudio
```
```
reboot
```
And sound was working again.
