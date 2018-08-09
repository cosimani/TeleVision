# -*- coding: utf-8 -*-
"""
Created on Tue Jul 31 09:31:57 2018

@author: cosimani
"""

import numpy as np
from skimage import io
from sklearn.cluster import KMeans
import sys
import cv2

dir_de_trabajo = sys.argv[ 1 ]
ruta_completa_imagen = sys.argv[ 2 ]
nro_clusters = int( sys.argv[ 3 ] )
donde_guardar_im_procesada = sys.argv[ 4 ]

if nro_clusters == 2 :
    palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ] ] )
elif nro_clusters == 3 :
    palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ] ] )
elif nro_clusters == 4 :
    palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ], [ 255, 0, 255 ] ] )
elif nro_clusters == 5 :
    palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ], 
                          [ 255, 0, 255 ], [ 255, 255, 255 ] ] )    
elif nro_clusters == 6 :
    palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ], 
                          [ 255, 0, 255 ], [ 255, 255, 255 ], [ 0, 255, 255 ] ] )    
else :  # Si por algun motivo esta fuera de rango, por defecto se toma 4
    nro_clusters = 4
    palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ], [ 255, 0, 255 ] ] )
    
img = io.imread( ruta_completa_imagen )

# Si es igual a 2, entonces es una imagen con una sola banda.
# Si es asi, entonces la transformamos en RGB, pero en escala de grises
if len( img.shape ) == 2 :
    img = cv2.cvtColor( img, cv2.COLOR_GRAY2RGB )
     
rows, cols, bands = img.shape
    
X = img.reshape( rows * cols, bands )

kmeans = KMeans( n_clusters = nro_clusters, random_state = 0 ).fit( X )
unsupervised = kmeans.labels_.reshape( rows, cols )

io.imsave( donde_guardar_im_procesada, palette[ unsupervised ] )

print ( donde_guardar_im_procesada )






