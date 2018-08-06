# -*- coding: utf-8 -*-
"""
Created on Tue Jul 31 09:31:57 2018

@author: cosimani
"""

import numpy as np
from skimage import io
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt

import sys
 
print "Número de parámetros: ", len(sys.argv)
print "Lista de argumentos: ", sys.argv


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


#img = io.imread('https://i.stack.imgur.com/TFOv7.png')
img = io.imread( dir_de_trabajo + nombreArchivo )
rows, cols, bands = img.shape

print ( 'rows=', rows, ' cols=', cols, ' bands=', bands )

plt.figure( num = 1, figsize = ( 12, 6 ) ) 
plt.title( 'Córdoba - 3 bandas' )
plt.imshow( img, vmin = 0, vmax = 255, cmap = 'gray' )
plt.axis( 'off' )


# No supervisado
X = img.reshape( rows * cols, bands )


kmeans = KMeans( n_clusters = n_classes, random_state = 0 ).fit( X )
unsupervised = kmeans.labels_.reshape( rows, cols )

plt.figure( num = 2, figsize = ( 12, 6 ) ) 
plt.title( 'Córdoba - No supervisado' )
io.imshow( palette[ unsupervised ] )
plt.axis( 'off' )


# Supervisado


supervised = n_classes * np.ones( shape = ( rows, cols ), dtype = np.int )
#supervised[ 200 : 220, 150 : 170 ] = classes[ 'building' ]
#supervised[ 40 : 60, 40 : 60 ] = classes[ 'vegetation' ]
#supervised[ 100 : 120, 200 : 220 ] = classes[ 'water' ]
supervised[ 95 : 144, 72 : 133 ] = classes[ 'building' ]
supervised[ 838 : 859, 621 : 644 ] = classes[ 'vegetation' ]
supervised[ 734 : 755, 506 : 529 ] = classes[ 'water' ]


print( supervised )

y = supervised.ravel()
train = np.flatnonzero( supervised < n_classes )
test = np.flatnonzero( supervised == n_classes )

from sklearn.svm import SVC

clf = SVC()

print( 'X.shape =', X.shape, ' y.shape =', y.shape, ' train =', train, ' test =', test )

clf.fit( X[ train ], y[ train ] )
y[ test ] = clf.predict( X[ test ] )
supervised = y.reshape( rows, cols )
#io.imshow( palette[ supervised ] )

plt.figure( num = 3, figsize = ( 12, 6 ) ) 
plt.title( 'Córdoba - Supervisado' )
io.imshow( palette[ supervised ] )
plt.axis( 'off' )










