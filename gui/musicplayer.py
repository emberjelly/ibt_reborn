#!/usr/bin/env python


#############################################################################
##
## Copyright (C) 2010 Riverbank Computing Limited.
## Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
## All rights reserved.
##
## This file is part of the examples of PyQt.
##
## $QT_BEGIN_LICENSE:BSD$
## You may use this file under the terms of the BSD license as follows:
##
## "Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are
## met:
##   * Redistributions of source code must retain the above copyright
##     notice, this list of conditions and the following disclaimer.
##   * Redistributions in binary form must reproduce the above copyright
##     notice, this list of conditions and the following disclaimer in
##     the documentation and/or other materials provided with the
##     distribution.
##   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
##     the names of its contributors may be used to endorse or promote
##     products derived from this software without specific prior written
##     permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
## "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
## LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
## A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
## OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
## SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
## LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
## DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
## THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
## (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
## OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
## $QT_END_LICENSE$
##
#############################################################################
import ntpath
#import subprocess
import os
import sched
import time
import compute_downbeats

# This is only needed for Python v2 but is harmless for Python v3.
import sip
sip.setapi('QString', 2)

import sys

from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import QThread

try:
    from PyQt4.phonon import Phonon
except ImportError:
    app = QtGui.QApplication(sys.argv)
    QtGui.QMessageBox.critical(None, "Music Player",
            "Your Qt installation does not have Phonon support.",
            QtGui.QMessageBox.Ok | QtGui.QMessageBox.Default,
            QtGui.QMessageBox.NoButton)
    sys.exit(1)



'''
class YourThreadName(QThread):

    def __init__(self, fname):
        QThread.__init__(self)

        tmp = os.path.split(fname)[1]
        
        base_path = os.path.split(fname)[0] + "/beats"        

        base = os.path.splitext(tmp)[0]


        self.fname = base_path + "/" + base + ".txt"

        print self.fname
        self.isrunning = True
        self.times = None
        self.beat_num = 0
        self.elapsed = 0

        f = open(self.fname, 'r')
        data = f.read()
        
        self.times = data.split('\n')
        self.times.pop(-1)

    def __del__(self):
        self.wait()

    def run(self):
        # your logic here

        #os.rename(self.fname, base + ".txt")
        f = open(self.fname, 'r')
        scheduler = sched.scheduler(time.time, time.sleep)

        def print_event(name):
            #print "Beat at " + self.times[self.beat_num] + " " + str(self.elapsed)
            self.beat_num += 1

            error = self.elapsed/1000.0 - float(self.times[self.beat_num])
            #print error

            if self.isrunning:
                scheduler.enter(float(self.times[self.beat_num]) - float(self.times[self.beat_num - 1]) - error, 1, print_event, (self.beat_num,))
            else:
                self.beat_num = 0

        #for i, time_ in enumerate(times):

        scheduler.enter(float(self.times[0]), 1, print_event, (1,))

        scheduler.run()
'''
import subprocess

def get_beats(fname):

    file_path = fname
    print "Loading ", fname
    tmp = os.path.split(fname)[1]
    base_path = os.path.split(fname)[0] + "/beats"        
    base = os.path.splitext(tmp)[0]
    fname = base_path + "/" + base + ".txt"

    if not os.path.isfile(fname):
        bashCommand = ["essentia_bin/streaming_beattracker_multifeature_mirex2013", file_path,  fname]
        subprocess.call(bashCommand)



    f = open(fname, 'r')
    data = f.read()

    beats = data.split('\n')
    beats.pop(-1)
    beats = map(float, beats)
    beats.append(99999)
    return beats

def get_downbeats(fname):

    print "Loading", fname

    ttmp = fname;
    tmp = os.path.split(fname)[1]
    base_path = os.path.split(fname)[0] + "/annotated_downbeats"  
    base = os.path.splitext(tmp)[0]
    fname = base_path + "/" + base + ".txt"



    if os.path.isfile(fname):

        print "Loading saved down beats"
        f = open(fname, 'r')
        data = f.read()

        beats = data.split('\n')
        beats.pop(-1)
        beats = map(int, beats)
        return beats
    else:

        print "Loading estimted down beats"

        fname = ttmp
        tmp = os.path.split(fname)[1]
        base_path = os.path.split(fname)[0] + "/downbeats"        
        base = os.path.splitext(tmp)[0]
        fname = base_path + "/" + base + ".txt"

        if not os.path.isfile(fname):
            return [1, 2, 3, 4]*1000
        else:

            (beat_times, beat_nums) = compute_downbeats.load_down_beat_times(fname)

            fname = ttmp
            tmp = os.path.split(fname)[1]
            base_path = os.path.split(fname)[0] + "/beats"        
            base = os.path.splitext(tmp)[0]
            fname = base_path + "/" + base + ".txt"
            f = open(fname, 'r')
            data = f.read()

            beats = data.split('\n')
            beats.pop(-1)
            beats = map(float, beats)
            beat_times = map(float, beat_times)
            beat_nums = map(int, beat_nums)

            return compute_downbeats.compute_downbeats(beats, beat_times, beat_nums)
        


class MainWindow(QtGui.QMainWindow):
    def __init__(self):
        super(QtGui.QMainWindow, self).__init__()

        self.audioOutput = Phonon.AudioOutput(Phonon.MusicCategory, self)
        self.mediaObject = Phonon.MediaObject(self)
        self.metaInformationResolver = Phonon.MediaObject(self)

        self.mediaObject.setTickInterval(10)

        self.mediaObject.tick.connect(self.tick)
        self.mediaObject.stateChanged.connect(self.stateChanged)
        self.metaInformationResolver.stateChanged.connect(self.metaStateChanged)
        self.mediaObject.currentSourceChanged.connect(self.sourceChanged)
        self.mediaObject.aboutToFinish.connect(self.aboutToFinish)

        Phonon.createPath(self.mediaObject, self.audioOutput)

        self.setupActions()
        self.setupMenus()
        self.setupUi()
        self.timeLcd.display("00:00") 

        self.sources = []

        #self.myThread = YourThreadName("./tmp.txt")

        self.tick_count = 0
        self.beat_num = 0;
        self.beats = [0, 1]
        self.downbeats = [1, 2]

        self.current_fname = ""


        #MainWindow.keyPressEvent = self.newOnkeyPressEvent

        self.p_text = QtGui.QPlainTextEdit()
        self.p_text.keyPressEvent = self.keyPressEvent
        '''
        default_folder = '/home/jerry/Music'

        index = 1
        for string in os.listdir(default_folder):
            if string.endswith(".mp3"):
            #print(os.path.join("/mydir", file))
                self.sources.append(Phonon.MediaSource(string))
                filename = ntpath.basename(string)
                titleItem = QtGui.QTableWidgetItem(filename)
                titleItem.setFlags(titleItem.flags() ^ QtCore.Qt.ItemIsEditable)

                currentRow = self.musicTable.rowCount()
                self.musicTable.insertRow(currentRow)
                self.musicTable.setItem(currentRow, 0, titleItem)
        
        if self.sources:
            self.metaInformationResolver.setCurrentSource(self.sources[index])
        '''
    def keyPressEvent(self,e):
        
        print e.key()
        if e.key() == 16777223:
            print "Saving new down beats"
            fname = self.current_fname
            tmp = os.path.split(fname)[1]
            base_path = os.path.split(fname)[0] + "/annotated_downbeats"        
            base = os.path.splitext(tmp)[0]
            fname = base_path + "/" + base + ".txt"

            first_beat = self.beat_num%4
            first_beat = (first_beat + 1) % 4


            mapping = [1, 4, 3, 2]
            first_beat = mapping[first_beat]


            output_nums = ""


            for i in range(len(self.beats)):
                output_nums = output_nums + str(first_beat) + "\n" 
                first_beat = (first_beat%4) + 1


            print fname
            f = open(fname, 'w')
            f.write(output_nums)
            f.close()
            self.downbeats = get_downbeats(self.current_fname)


    
    def sizeHint(self):
        return QtCore.QSize(800, 600)

    def addFiles(self):
        files = QtGui.QFileDialog.getOpenFileNames(self, "Select Music Files",
                QtGui.QDesktopServices.storageLocation(QtGui.QDesktopServices.MusicLocation))

        if not files:
            return

        index = len(self.sources)

        for string in files:
            self.sources.append(Phonon.MediaSource(string))
            filename = ntpath.basename(string)
            titleItem = QtGui.QTableWidgetItem(filename)
            titleItem.setFlags(titleItem.flags() ^ QtCore.Qt.ItemIsEditable)

            currentRow = self.musicTable.rowCount()
            self.musicTable.insertRow(currentRow)
            self.musicTable.setItem(currentRow, 0, titleItem)


        if self.sources:
            self.metaInformationResolver.setCurrentSource(self.sources[index])

    def about(self):
        QtGui.QMessageBox.information(self, "About Music Player",
                "The Music Player example shows how to use Phonon - the "
                "multimedia framework that comes with Qt - to create a "
                "simple music player.")

    def stateChanged(self, newState, oldState):

        #print self.metaInformationResolver.currentSource().fileName()
        
        if newState == Phonon.ErrorState:
            if self.mediaObject.errorType() == Phonon.FatalError:
                QtGui.QMessageBox.warning(self, "Fatal Error",
                        self.mediaObject.errorString())
            else:
                QtGui.QMessageBox.warning(self, "Error",
                        self.mediaObject.errorString())

        elif newState == Phonon.PlayingState:
            self.playAction.setEnabled(False)
            self.pauseAction.setEnabled(True)
            self.stopAction.setEnabled(True)
            print("Start IBT")
            #self.myThread = YourThreadName(self.metaInformationResolver.currentSource().fileName())
        
            
            self.beats = get_beats(self.current_fname)
            self.downbeats = get_downbeats(self.current_fname)
            #(self.beats, self.downbeats) = compute_downbeats.load_down_beat_times(self.metaInformationResolver.currentSource().fileName())
            #print self.beats
            #print self.beats
            #self.myThread.isrunning = True
            #self.myThread.start()



        elif newState == Phonon.StoppedState:
            self.stopAction.setEnabled(False)
            self.playAction.setEnabled(True)
            self.pauseAction.setEnabled(False)
            self.timeLcd.display("00:00")
            #self.myThread.isrunning = False
            print("Stop IBT")
            self.beat_num = 0

        elif newState == Phonon.PausedState:
            self.pauseAction.setEnabled(False)
            self.stopAction.setEnabled(True)
            self.playAction.setEnabled(True)
            #self.myThread.isrunning = False
            print("Pause IBT")

    def tick(self, time):
        
           
        self.tick_count += 1

        if self.tick_count == 100:
            displayTime = QtCore.QTime(0, (time / 60000) % 60, (time / 1000) % 60)
            self.timeLcd.display(displayTime.toString('mm:ss'))
            self.tick_count = 0

        #print time/1000.0 - 0.05
        #print  self.beats[self.beat_num]

        while (self.beats[self.beat_num] - time/1000.0 > 1):
            self.beat_num -= 1


        if time/1000.0 + 0.005 > self.beats[self.beat_num]:
            while (time/1000.0 + 0.005 > self.beats[self.beat_num]):
                self.beat_num += 1
            print "Current Beat", self.downbeats[self.beat_num], "Beat Time: ", self.beats[self.beat_num - 1], "Beat Num: ", str(self.beat_num).zfill(2), "Error: ", time/1000.0 - self.beats[self.beat_num - 1]
            


        #self.myThread.elapsed = time

    def tableClicked(self, row, column):
        wasPlaying = (self.mediaObject.state() == Phonon.PlayingState)

        #self.mediaObject.stop()
        #self.mediaObject.clearQueue()

        self.current_fname = self.sources[row].fileName()
        self.mediaObject.setCurrentSource(self.sources[row])

        #if wasPlaying:
        #    self.mediaObject.play()
        #else:
        #    self.mediaObject.stop()

    def sourceChanged(self, source):
        self.musicTable.selectRow(self.sources.index(source))

        self.timeLcd.display('00:00')

        

    def metaStateChanged(self, newState, oldState):
        if newState == Phonon.ErrorState:
            QtGui.QMessageBox.warning(self, "Error opening files",
                    self.metaInformationResolver.errorString())

            while self.sources and self.sources.pop() != self.metaInformationResolver.currentSource():
                pass

            return

        if newState != Phonon.StoppedState and newState != Phonon.PausedState:
            return

        if self.metaInformationResolver.currentSource().type() == Phonon.MediaSource.Invalid:
            return

        metaData = self.metaInformationResolver.metaData()

        #title = metaData.get('TITLE', [''])[0]
        #if not title:
        #    title = self.metaInformationResolver.currentSource().fileName()

        #titleItem = QtGui.QTableWidgetItem(title)
        #titleItem.setFlags(titleItem.flags() ^ QtCore.Qt.ItemIsEditable)

        #artist = metaData.get('ARTIST', [''])[0]
        #artistItem = QtGui.QTableWidgetItem(artist)
        #artistItem.setFlags(artistItem.flags() ^ QtCore.Qt.ItemIsEditable)

        #album = metaData.get('ALBUM', [''])[0]
        #albumItem = QtGui.QTableWidgetItem(album)
        #albumItem.setFlags(albumItem.flags() ^ QtCore.Qt.ItemIsEditable)

        #year = metaData.get('DATE', [''])[0]
        #yearItem = QtGui.QTableWidgetItem(year)
        #yearItem.setFlags(yearItem.flags() ^ QtCore.Qt.ItemIsEditable)

        #currentRow = self.musicTable.rowCount()
        #self.musicTable.insertRow(currentRow)
        #self.musicTable.setItem(currentRow, 0, titleItem)
        #self.musicTable.setItem(currentRow, 1, artistItem)
        #self.musicTable.setItem(currentRow, 2, albumItem)
        #self.musicTable.setItem(currentRow, 3, yearItem)

        #if not self.musicTable.selectedItems():
        #    self.musicTable.selectRow(0)
        #    self.mediaObject.setCurrentSource(self.metaInformationResolver.currentSource())

        #index = self.sources.index(self.metaInformationResolver.currentSource()) + 1

        #if len(self.sources) > index:
        #    self.metaInformationResolver.setCurrentSource(self.sources[index])
        #else:
        #    self.musicTable.resizeColumnsToContents()
        #    if self.musicTable.columnWidth(0) > 300:
        #        self.musicTable.setColumnWidth(0, 300)

    def aboutToFinish(self):
        index = self.sources.index(self.mediaObject.currentSource()) + 1
        if len(self.sources) > index:
            self.mediaObject.enqueue(self.sources[index])

    def setupActions(self):
        self.playAction = QtGui.QAction(
                self.style().standardIcon(QtGui.QStyle.SP_MediaPlay), "Play",
                self, shortcut="Ctrl+P", enabled=False,
                triggered=self.mediaObject.play)

        self.pauseAction = QtGui.QAction(
                self.style().standardIcon(QtGui.QStyle.SP_MediaPause),
                "Pause", self, shortcut="Ctrl+A", enabled=False,
                triggered=self.mediaObject.pause)

        self.stopAction = QtGui.QAction(
                self.style().standardIcon(QtGui.QStyle.SP_MediaStop), "Stop",
                self, shortcut="Ctrl+S", enabled=False,
                triggered=self.mediaObject.stop)

        self.nextAction = QtGui.QAction(
                self.style().standardIcon(QtGui.QStyle.SP_MediaSkipForward),
                "Next", self, shortcut="Ctrl+N")

        self.previousAction = QtGui.QAction(
                self.style().standardIcon(QtGui.QStyle.SP_MediaSkipBackward),
                "Previous", self, shortcut="Ctrl+R")

        self.addFilesAction = QtGui.QAction("Add &Files", self,
                shortcut="Ctrl+O", triggered=self.addFiles)

        self.exitAction = QtGui.QAction("E&xit", self, shortcut="Ctrl+X",
                triggered=self.close)

        self.aboutAction = QtGui.QAction("A&bout", self, shortcut="Ctrl+B",
                triggered=self.about)

        self.aboutQtAction = QtGui.QAction("About &Qt", self,
                shortcut="Ctrl+Q", triggered=QtGui.qApp.aboutQt)

    def setupMenus(self):
        fileMenu = self.menuBar().addMenu("&File")
        fileMenu.addAction(self.addFilesAction)
        fileMenu.addSeparator()
        fileMenu.addAction(self.exitAction)

        aboutMenu = self.menuBar().addMenu("&Help")
        aboutMenu.addAction(self.aboutAction)
        aboutMenu.addAction(self.aboutQtAction)

    def setupUi(self):
        bar = QtGui.QToolBar()

        bar.addAction(self.playAction)
        bar.addAction(self.pauseAction)
        bar.addAction(self.stopAction)

        self.seekSlider = Phonon.SeekSlider(self)
        self.seekSlider.setMediaObject(self.mediaObject)

        self.volumeSlider = Phonon.VolumeSlider(self)
        self.volumeSlider.setAudioOutput(self.audioOutput)
        self.volumeSlider.setSizePolicy(QtGui.QSizePolicy.Maximum,
                QtGui.QSizePolicy.Maximum)

        volumeLabel = QtGui.QLabel()
        volumeLabel.setPixmap(QtGui.QPixmap('images/volume.png'))

        palette = QtGui.QPalette()
        palette.setBrush(QtGui.QPalette.Light, QtCore.Qt.darkGray)

        self.timeLcd = QtGui.QLCDNumber()
        self.timeLcd.setPalette(palette)

        headers = (["Playlist"])

        self.musicTable = QtGui.QTableWidget(0, 1)

        self.musicTable.setHorizontalHeaderLabels(headers)
        self.musicTable.setSelectionMode(QtGui.QAbstractItemView.SingleSelection)
        self.musicTable.setSelectionBehavior(QtGui.QAbstractItemView.SelectRows)
        self.musicTable.cellPressed.connect(self.tableClicked)
        self.musicTable.setColumnWidth(0, 800)


        seekerLayout = QtGui.QHBoxLayout()
        seekerLayout.addWidget(self.seekSlider)
        seekerLayout.addWidget(self.timeLcd)

        playbackLayout = QtGui.QHBoxLayout()
        playbackLayout.addWidget(bar)
        playbackLayout.addStretch()
        playbackLayout.addWidget(volumeLabel)
        playbackLayout.addWidget(self.volumeSlider)

        mainLayout = QtGui.QVBoxLayout()
        mainLayout.addWidget(self.musicTable)
        mainLayout.addLayout(seekerLayout)
        mainLayout.addLayout(playbackLayout)

        widget = QtGui.QWidget()
        widget.setLayout(mainLayout)

        self.setCentralWidget(widget)
        self.setWindowTitle("Phonon Music Player")



if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    app.setApplicationName("Music Player")
    app.setQuitOnLastWindowClosed(True)

    window = MainWindow()
    window.show()

    sys.exit(app.exec_())