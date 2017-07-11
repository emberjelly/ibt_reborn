'''
import musicplayer, sys, os, fnmatch, random, pprint, Tkinter

class Song:
        def __init__(self, fn):
                self.url = fn
                self.f = open(fn)
        # `__eq__` is used for the peek stream management
        def __eq__(self, other):
                return self.url == other.url
        # this is used by the player as the data interface
        def readPacket(self, bufSize):
                return self.f.read(bufSize)
        def seekRaw(self, offset, whence):
                r = self.f.seek(offset, whence)
                return self.f.tell()

files = []
def getFiles(path):
        for f in sorted(os.listdir(path), key=lambda k: random.random()):
                f = os.path.join(path, f)
                if os.path.isdir(f): getFiles(f) # recurse
                if len(files) > 1000: break # break if we have enough
                if fnmatch.fnmatch(f, '*.mp3'): files.append(f)
getFiles(os.path.expanduser("~/Music"))
random.shuffle(files) # shuffle some more

i = 0

def songs():
        global i, files
        while True:
                yield Song(files[i])
                i += 1
                if i >= len(files): i = 0

def peekSongs(n):
        nexti = i + 1
        if nexti >= len(files): nexti = 0
        return map(Song, (files[nexti:] + files[:nexti])[:n])

# Create our Music Player.
player = musicplayer.createPlayer()
player.outSamplerate = 96000 # support high quality :)
player.queue = songs()
player.peekQueue = peekSongs

# Setup a simple GUI.
window = Tkinter.Tk()
window.title("Music Player")
songLabel = Tkinter.StringVar()

def onSongChange(**kwargs): songLabel.set(pprint.pformat(player.curSongMetadata))
def cmdPlayPause(*args): player.playing = not player.playing
def cmdNext(*args): player.nextSong()

Tkinter.Label(window, textvariable=songLabel).pack()
Tkinter.Button(window, text="Play/Pause", command=cmdPlayPause).pack()
Tkinter.Button(window, text="Next", command=cmdNext).pack()

player.onSongChange = onSongChange
player.playing = True # start playing
window.mainloop()
'''

import musicplayer

musicplayer.createPlayer()