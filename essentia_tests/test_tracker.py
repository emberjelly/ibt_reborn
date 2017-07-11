import os

'''
import essentia
import essentia.standard


#print dir(essentia.standard)

help(essentia.standard.BeatTrackerMultiFeature)



essentia.standard.BeatTrackerMultiFeature('RedHotChiliPeppers-DaniCalifornia.mp3')
'''
'''
import subprocess

bashCommand = ["essentia_bin/streaming_beattracker_multifeature_mirex2013", "../../Nirvana - Lithium.mp3",  "tmp.txt"]
subprocess.call(bashCommand)

f = open('tmp.txt', 'r')
data = f.read()
print data.split('\n')
'''

import sched
import time


f = open('tmp.txt', 'r')
data = f.read()

scheduler = sched.scheduler(time.time, time.sleep)

def print_event(name):
    print 'EVENT:', time.time(), name

print 'START:', time.time()

times = data.split('\n')
times.pop(-1)

for time_ in times:

	scheduler.enter(float(time_.strip()), 1, print_event, ('first',))

scheduler.run()