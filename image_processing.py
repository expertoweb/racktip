import sys, pygame, os, time
import matplotlib
matplotlib.use('Agg') # Must be before importing matplotlib.pyplot or pylab!
import matplotlib.pyplot as plt
import RPi.GPIO as GPIO
import pygame.camera
import numpy as np
import scipy
from pygame.locals import *
from skimage import data
from scipy import ndimage
from skimage import io
from skimage.draw import ellipse
from skimage.morphology import label
from skimage.measure import regionprops


WIDTH = 128
HEIGHT = 160

def load_image(filename, transparent=False):
        try: image = pygame.image.load(filename)
        except pygame.error, message:
                raise SystemExit, message
        image = image.convert()
        if transparent:
                color = image.get_at((0,0))
                image.set_colorkey(color, RLEACCEL)
        return image
 

def getPixelArray(filename):
    """ Open file, load image and convert it to 3D pixel array. """

    try:
        image = pygame.image.load(filename)
    except pygame.error, message:
        print "Cannot load image:", filename
        raise SystemExit, message
    
    return pygame.surfarray.array3d(image)

def saveSurface(pixels, filename):
    """ Convert 3D array of pixels to Pygame surface and save. """
    
    try:
        surf = pygame.surfarray.make_surface(pixels)
    except IndexError:
        (width, height, colours) = pixels
        surf = pygame.display.set_mode((width, height))
        pygame.surfarray.blit_array(surf, pixels)
    
    pygame.image.save(surf, filename)


def main():
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Pruebas Pygame")
    pygame.mouse.set_visible(0)

    clock = pygame.time.Clock()
    cam = pygame.camera.Camera("/dev/video0",(320,240))
    cam.start()
    while True:
        image = cam.get_image()
        pygame.image.save(image, './webcam.jpg')
        pixels = getPixelArray('./webcam.jpg')
        pixels[100] = 0
        pixels = pixels[::2,::2,:]

        saveSurface(pixels, './webcam2.jpg')

        #GPIO.output(4, False)
        #ticks = clock.tick(60)
        GPIO.output(4, True)

        for eventos in pygame.event.get():
            if eventos.type == QUIT:
                sys.exit(0)


	lena =  io.imread("./webcam2.jpg")
	# Cropping
	crop_lena = lena[100: - 100, 100: - 100]
	# up <-> down flip
	flip_ud_lena = np.flipud(crop_lena)
	# rotation
	rotate_lena = ndimage.rotate(lena, 45)
	plt.imshow(rotate_lena, cmap=plt.cm.gray)

        fig = matplotlib.pyplot.gcf()
        fig.set_size_inches(1,1)
        fig.savefig('test2png.png',dpi=100)
        plt.savefig("plot.jpg")

        image = load_image('./plot.jpg')
        screen.blit(image, (0, 0))
        pygame.display.flip()
    return 0
if __name__ == '__main__':
    os.environ["SDL_FBDEV"] = "/dev/fb1"
    pygame.init()
    pygame.camera.init()
    #Setup GPIO
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(4, GPIO.OUT) #Read GPIO 4 as input

    main()

    cam.stop()
    GPIO.cleanup()
