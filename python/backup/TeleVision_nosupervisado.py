# -*- coding: utf-8 -*-
"""
Created on Tue Jul 31 09:31:57 2018

@author: cosimani
"""

import numpy as np
from skimage import io
from sklearn.cluster import KMeans
import sys
from PIL import Image

def pegar_imagenes( file1, file2, donde_almacenar ) :
    list_im = [ file1, file2 ]
    
    im1 = Image.open( file1 )
    im2 = Image.open( file2 )
    
    width1, height1 = im1.size    
    width2, height2 = im2.size    
    
    # Solo pega las imagenes si tienen las mismas dimensiones
    if width1 == width2 and height1 == height2 :
        
        new_im = Image.new( 'RGB', ( width1 * 2, height1 ), "white" )

        im_1 = Image.open( file1 )        
        new_im.paste( im_1, ( 0, 0, width1, height1 ) )
        
        im_2 = Image.open( file2 )        
        new_im.paste( im_2, ( width1, 0, width2, height2 ) )

#        for elem in list_im:
#            for i in range( 0, width1 * 2, height1 ) :
#                im = Image.open( elem )
#                new_im.paste( im, ( i, 0 ) )
        new_im.save( donde_almacenar )
        
    else :
        return    
        

def concat_images( imga, imgb ):
    """
    Combines two color image ndarrays side-by-side.
    """
    ha, wa = imga.shape[ : 2 ]
    hb, wb = imgb.shape[ : 2 ]
    max_height = np.max( [ ha, hb ] )
    total_width = wa + wb
    new_img = np.zeros( shape = ( max_height, total_width, 3 ) )
    new_img[ : ha, : wa ] = imga
    new_img[ : hb, wa : wa + wb ] = imgb
    return new_img
 
#print ( 'Número de parámetros: ', len( sys.argv ) )
#print ( 'Lista de argumentos: ', sys.argv )
#
#print ( sys.argv[ 0 ] )

dir_de_trabajo = sys.argv[ 1 ]
ruta_completa_imagen = sys.argv[ 2 ]
nro_clusters = int( sys.argv[ 3 ] )
donde_guardar_im_procesada = sys.argv[ 4 ]


# Los parametros que se reciben son:

#directorio del proyecto: por ejemplo: /home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision
#ruta completa de la imagen: por ejemplo /home/cosimani/pictures/im.png
#clusters de K-Means
#donde almacenar la resultante: 
#   por ejemplo: /home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision/processedImages/unsupervised/im_001.png

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
    

#classes = { 'vegetation': 0, 'building': 1, 'water': 2 }
#n_classes = len( classes )
#palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ] ] )

#n_classes = 3
# # Para 3 classes
#palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ] ] )


# # Para 4 classes
#palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ], 
#                      [ 0, 255, 255 ] ] )

# # Para 5 classes
#palette = np.uint8( [ [ 0, 255, 0 ], [ 255, 0, 0 ], [ 0, 0, 255 ], 
#                      [ 0, 255, 255 ], [ 255, 0, 255 ] ] )


#dir_de_trabajo = '/home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision/images/'
#nombreArchivo = 'cordoba457_cesar.tif'   

#dir_proyecto = '/home/cosimani/Proyecto/2018/Curso FAMAF/TeleVisión/TeleVision/'


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

kmeans = KMeans( n_clusters = nro_clusters, random_state = 0 ).fit( X )
unsupervised = kmeans.labels_.reshape( rows, cols )

#plt.figure( num = 2, figsize = ( 12, 6 ) ) 
#plt.title( 'Córdoba - No supervisado' )
#io.imshow( palette[ unsupervised ] )
#plt.axis( 'off' )

#filename, file_extension = os.path.splitext( nombreArchivo )
#print ( dir_de_trabajo + filename + '_001' + file_extension )

#dir_almacenamiento = 'processedImages/unsupervised/'


#io.imsave( dir_proyecto + dir_almacenamiento + filename + '_001' + file_extension, palette[ unsupervised ] )
io.imsave( donde_guardar_im_procesada, palette[ unsupervised ] )

print ( donde_guardar_im_procesada )

#img_procesada = io.imread( donde_guardar_im_procesada )

#pegar_imagenes( ruta_completa_imagen, donde_guardar_im_procesada, donde_guardar_im_procesada + '.jpg' )

#output = concat_images( img, img_procesada )
#io.imsave( donde_guardar_im_procesada + '.jpg', output )









