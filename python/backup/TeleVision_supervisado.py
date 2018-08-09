# -*- coding: utf-8 -*-
"""
Created on Tue Jul 31 09:31:57 2018

@author: cosimani
"""

import numpy as np
from skimage import io
from sklearn.svm import SVC
import sys
 
dir_de_trabajo = sys.argv[ 1 ]
ruta_completa_imagen = sys.argv[ 2 ]
n_classes = int( sys.argv[ 3 ] )
donde_guardar_im_procesada = sys.argv[ 4 ]

x1Clasif1 = int( sys.argv[ 5 ] )
x2Clasif1 = int( sys.argv[ 6 ] )
y1Clasif1 = int( sys.argv[ 7 ] )
y2Clasif1 = int( sys.argv[ 8 ] )

x1Clasif2 = int( sys.argv[ 9 ] )
x2Clasif2 = int( sys.argv[ 10 ] )
y1Clasif2 = int( sys.argv[ 11 ] )
y2Clasif2 = int( sys.argv[ 12 ] )

x1Clasif3 = int( sys.argv[ 13 ] )
x2Clasif3 = int( sys.argv[ 14 ] )
y1Clasif3 = int( sys.argv[ 15 ] )
y2Clasif3 = int( sys.argv[ 16 ] )

x1Clasif4 = int( sys.argv[ 17 ] )
x2Clasif4 = int( sys.argv[ 18 ] )
y1Clasif4 = int( sys.argv[ 19 ] )
y2Clasif4 = int( sys.argv[ 20 ] )


if n_classes == 2 :
    palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ] ] )
    classes = { 'Clasif1': 0, 'Clasif2': 1 }
elif n_classes == 3 :
    palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ] ] )
    classes = { 'Clasif1': 0, 'Clasif2': 1, 'Clasif3': 2 }
elif n_classes == 4 :
    palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ], [ 255, 0, 255 ] ] )
    classes = { 'Clasif1': 0, 'Clasif2': 1, 'Clasif3': 2, 'Clasif4': 3 }
else :  # Si por algun motivo esta fuera de rango, por defecto se toma 4
    n_classes = 4
    palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ], [ 255, 0, 255 ] ] )
    classes = { 'Clasif1': 0, 'Clasif2': 1, 'Clasif3': 2, 'Clasif4': 3 }


#img = io.imread('https://i.stack.imgur.com/TFOv7.png')
img = io.imread( ruta_completa_imagen )
rows, cols, bands = img.shape

# No supervisado
X = img.reshape( rows * cols, bands )

# Supervisado
supervised = n_classes * np.ones( shape = ( rows, cols ), dtype = np.int )

if n_classes == 2 :
    supervised[ x1Clasif1 : x2Clasif1, y1Clasif1 : y2Clasif1 ] = classes[ 'Clasif1' ]
    supervised[ x1Clasif2 : x2Clasif2, y1Clasif2 : y2Clasif2 ] = classes[ 'Clasif2' ]
elif n_classes == 3 :
    supervised[ x1Clasif1 : x2Clasif1, y1Clasif1 : y2Clasif1 ] = classes[ 'Clasif1' ]
    supervised[ x1Clasif2 : x2Clasif2, y1Clasif2 : y2Clasif2 ] = classes[ 'Clasif2' ]
    supervised[ x1Clasif3 : x2Clasif3, y1Clasif3 : y2Clasif3 ] = classes[ 'Clasif3' ]
elif n_classes == 4 :
    supervised[ x1Clasif1 : x2Clasif1, y1Clasif1 : y2Clasif1 ] = classes[ 'Clasif1' ]
    supervised[ x1Clasif2 : x2Clasif2, y1Clasif2 : y2Clasif2 ] = classes[ 'Clasif2' ]
    supervised[ x1Clasif3 : x2Clasif3, y1Clasif3 : y2Clasif3 ] = classes[ 'Clasif3' ]
    supervised[ x1Clasif4 : x2Clasif4, y1Clasif4 : y2Clasif4 ] = classes[ 'Clasif4' ]
else :  # Si por algun motivo esta fuera de rango, por defecto se toma 4
    n_classes = 4
    supervised[ x1Clasif1 : x2Clasif1, y1Clasif1 : y2Clasif1 ] = classes[ 'Clasif1' ]
    supervised[ x1Clasif2 : x2Clasif2, y1Clasif2 : y2Clasif2 ] = classes[ 'Clasif2' ]
    supervised[ x1Clasif3 : x2Clasif3, y1Clasif3 : y2Clasif3 ] = classes[ 'Clasif3' ]
    supervised[ x1Clasif4 : x2Clasif4, y1Clasif4 : y2Clasif4 ] = classes[ 'Clasif4' ]

y = supervised.ravel()
train = np.flatnonzero( supervised < n_classes )
test = np.flatnonzero( supervised == n_classes )

clf = SVC()

#print( 'X.shape =', X.shape, ' y.shape =', y.shape, ' train =', train, ' test =', test )

clf.fit( X[ train ], y[ train ] )
y[ test ] = clf.predict( X[ test ] )
supervised = y.reshape( rows, cols )
#io.imshow( palette[ supervised ] )

io.imsave( donde_guardar_im_procesada, palette[ supervised ] )

print ( donde_guardar_im_procesada )









