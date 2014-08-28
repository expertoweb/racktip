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
from skimage.color import rgb2gray
from skimage.draw import ellipse
from skimage.feature import peak

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

        #ticks = clock.tick(60)
        GPIO.output(4, True)

        for eventos in pygame.event.get():
            if eventos.type == QUIT:
                sys.exit(0)

	img =  io.imread("./webcam2.jpg")
        lx, ly, type = img.shape
	# Cropping
	crop_img = img[lx/4:-lx/4, ly/4:-ly/4]
	# up <-> down flip
	# flip_ud_lena = np.flipud(crop_lena)
	# rotation
	# rotate_lena = ndimage.rotate(lena, 45)
	# plt.imshow(rotate_lena, cmap=plt.cm.gray)

	image_gray = rgb2gray(img)

	#blobs_log = blob_log(image_gray, max_sigma=30, num_sigma=10, threshold=.1)
	# Compute radii in the 3rd column.
	#blobs_log[:, 2] = blobs_log[:, 2] * sqrt(2)

	#blobs_dog = blob_dog(image_gray, max_sigma=30, threshold=.1)
	#blobs_dog[:, 2] = blobs_dog[:, 2] * sqrt(2)

	#blobs_doh = blob_doh(image_gray, max_sigma=30, threshold=.01)

	#blobs_list = [blobs_log, blobs_dog, blobs_doh]
	colors = ['yellow', 'lime', 'red']
	titles = ['Laplacian of Gaussian', 'Difference of Gaussian', 'Determinant of Hessian']
	#sequence = zip(blobs_list, colors, titles)

	#for blobs, color, title in sequence:
    	#	fig, ax = plt.subplots(1, 1)
    	#	ax.set_title(title)
    	#	ax.imshow(image, interpolation='nearest')
    	#	for blob in blobs:
        #		y, x, r = blob
        #		c = plt.Circle((x, y), r, color=color, linewidth=2, fill=False)
        #		ax.add_patch(c)


	plt.figure(figsize=(1.5, 1.5))

	plt.axis('off')
	plt.imshow(crop_img, cmap=plt.cm.gray)

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
