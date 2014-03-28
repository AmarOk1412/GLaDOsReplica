import time
import cv2
import cv
import serial
import numpy
import os


#Constantes
TRAINSET = "lbpcascade_frontalface.xml"    #Fichier de reconnaissance
THRESHOLD = 90                             #Seuil de reconnaissance
CAMERA = 0                                 #La camera
ARDUINO = False                            #Utiliser l'arduino ?
IMAGE_SIZE = 170                           #Normalisation des images de base

port = "/dev/ttyACM0"
ser = serial.Serial(port, 9600)
time.sleep(2)
ser.write('l')

class Demo():
    def __init__(self):
        self.rval = False            
        self.camera = cv2.VideoCapture(CAMERA)
        self.classifier = cv2.CascadeClassifier(TRAINSET)
        self.faceFrame = None

    def getFacesPos(self, frame):
        """Retourne la position des visages detectes de la forme [[x y w h]]"""
        faces = self.classifier.detectMultiScale(frame)

        if len(faces) > 0:        
            if faces[0][0] - len(frame[0])/2 < -2:
                sendSerial('a-')
                print("Gauche")
            if faces[0][0] - len(frame[0])/2 > 2:
                sendSerial('a+')
                print("droite")
            if faces[0][1] - len(frame)/2 < -2:
                sendSerial('b-')
                print("haut")
            if faces[0][1] - len(frame)/2 > 2:
                sendSerial('b+')
                print("bas")
            if faces[0][0] - len(frame[0])/2 == 0:
                print("parfait")    
            if faces[0][1] - len(frame)/2 == 0:
                print("parfait")
        return faces

    def drawDetectedFace(self, frame, faces):
        """Dessine un rectangle autour du visage detecte"""
        for f in faces: 
            x,y,w,h = [v for v in f]
            cv2.rectangle(frame, (x,y), (x+w, y+h), (0,140,255))
            self.LBPHBaseImage = self.getFaceFrame(frame, x, y, w, h)
        return frame

    def getFaceFrame(self, frame, x, y, w, h):
        """On recupere un rectangle (largeur, hauteur) (centreX, centreY)"""
        cropped = cv2.getRectSubPix(frame, (w, h), (x + w / 2, y + h / 2))
        grayscale = cv2.cvtColor(cropped, cv2.COLOR_BGR2GRAY)
        self.faceFrame = cv2.resize(grayscale, (IMAGE_SIZE, IMAGE_SIZE))
        return self.faceFrame

    def capture(self): 
        """Recupere le flux video"""       
        if self.camera.isOpened():
            (rval, frame) = self.camera.read()
        else:
            rval = False

        while rval:
            (rval, frame) = self.camera.read()
            frame = self.drawDetectedFace(frame, self.getFacesPos(frame))
            cv2.imshow("Demo GLaDOs Arduino Day", frame)
            key = cv2.waitKey(20)
            readFile()
            if key in [27, ord('Q'), ord('q')]: #esc / Q
                break

def sendSerial(message):
    ser.write(message)

def readFile():
    try:
        f = open('gladosO', 'r')
        for l in f:
            print(l[:len(l)-1])
            ser.write(l[:len(l)-1])
        f.close()
        os.remove('gladosO')
    except:
        return 0
    
#a+/a-
#b+/b-/bg/bd
#c -> detect stop
#g base gauche
#d base droite
#h haut centrer vers le haut
#j bas centrer vers le bas
#k stop centrer
#ln -> led orange
#lr -> led rouge
#i -> init


if __name__ == '__main__':
    createDB = Demo()
    createDB.capture()
