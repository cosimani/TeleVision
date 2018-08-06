# -*- coding: utf-8 -*-
"""
Created on Tue Jul 31 09:31:57 2018

@author: cosimani
"""

import numpy as np
from skimage import io
from sklearn.cluster import KMeans
#import matplotlib.pyplot as plt
#import os

import sys
 
#print ( 'Número de parámetros: ', len( sys.argv ) )
#print ( 'Lista de argumentos: ', sys.argv )
#
#print ( sys.argv[ 0 ] )

dir_de_trabajo = sys.argv[ 1 ]
ruta_completa_imagen = sys.argv[ 2 ]
nro_clusters = sys.argv[ 3 ]
donde_guardar_im_procesada = sys.argv[ 4 ]

# Los parametros que se reciben son:

#directorio del proyecto: por ejemplo: /home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision
#ruta completa de la imagen: por ejemplo /home/cosimani/pictures/im.png
#clusters de K-Means
#donde almacenar la resultante: 
#   por ejemplo: /home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision/processedImages/unsupervised/im_001.png


classes = { 'vegetation': 0, 'building': 1, 'water': 2 }
n_classes = len( classes )
palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ] ] )

#n_classes = 3

# # Para 3 classes
#palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ] ] )


# # Para 4 classes
#palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ], 
#                      [ 0, 255, 255 ] ] )

# # Para 5 classes
#palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ], 
#                      [ 0, 255, 255 ], [ 255, 0, 255 ] ] )


dir_de_trabajo = '/home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision/images/'
nombreArchivo = 'cordoba457_cesar.tif'   

dir_proyecto = '/home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision/'


#img = io.imread('https://i.stack.imgur.com/TFOv7.png')
#img = io.imread( dir_de_trabajo + nombreArchivo )
img = io.imread( ruta_completa_imagen )
rows, cols, bands = img.shape

#print ( 'rows=', rows, ' cols=', cols, ' bands=', bands )
#
#plt.figure( num = 1, figsize = ( 12, 6 ) ) 
#plt.title( 'Córdoba - 3 bandas' )
#plt.imshow( img, vmin = 0, vmax = 255, cmap = 'gray' )
#plt.axis( 'off' )


# No supervisado
X = img.reshape( rows * cols, bands )

kmeans = KMeans( n_clusters = n_classes, random_state = 0 ).fit( X )
unsupervised = kmeans.labels_.reshape( rows, cols )

#plt.figure( num = 2, figsize = ( 12, 6 ) ) 
#plt.title( 'Córdoba - No supervisado' )
#io.imshow( palette[ unsupervised ] )
#plt.axis( 'off' )

#filename, file_extension = os.path.splitext( nombreArchivo )
#print ( dir_de_trabajo + filename + '_001' + file_extension )

dir_almacenamiento = 'processedImages/unsupervised/'


#io.imsave( dir_proyecto + dir_almacenamiento + filename + '_001' + file_extension, palette[ unsupervised ] )
io.imsave( donde_guardar_im_procesada, palette[ unsupervised ] )

print ( donde_guardar_im_procesada )











