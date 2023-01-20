import random
import os
import time
import warnings
warnings.filterwarnings("ignore")

import numpy as np
import pandas as pd 
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
# %matplotlib inline
import plotly.express as px
        
import tensorflow as tf
from tensorflow import keras
import tensorflow_hub as hub

import keras_preprocessing # pip install Keras-Preprocessing
from keras_preprocessing import image
from keras_preprocessing.image import ImageDataGenerator
from keras.callbacks import ReduceLROnPlateau

from keras import Sequential
from keras.preprocessing.image import ImageDataGenerator
from keras.optimizers import adam_v2
from keras.callbacks import EarlyStopping, ModelCheckpoint
from keras.layers import Flatten, Dense, BatchNormalization, Activation,Dropout

from tensorflow.keras.utils import plot_model
from tensorflow.keras import Model 

TRAINING_DIR = './input/clothing-dataset/images_compressed'
BATCH_SIZE = 32
IMAGE_SIZE = (224, 224)
EPOCHS = 100
LEARNING_RATE = 0.001

print("Num GPUs Available: ", len(tf.config.list_physical_devices('GPU')))

# Data prerprocessing 
file_names = os.listdir(TRAINING_DIR)
file_names[:10]

data = pd.read_csv('./input/clothing-dataset/images.csv')
data.head()

data['image'] = data['image']+'.jpg'
data_1 = data[['image', 'label']]

data_1.head()
print(data_1['image'])

import PIL
from pathlib import Path
from PIL import UnidentifiedImageError

path = Path(TRAINING_DIR).rglob("*.jpg")
for img_p in path:
    try:
        img = PIL.Image.open(img_p)
    except PIL.UnidentifiedImageError:
            print(img_p)

corrupted = ['b72ed5cd-9f5f-49a7-b12e-63a078212a17.jpg', '1d0129a1-f29a-4a3f-b103-f651176183eb.jpg', 'c60e486d-10ed-4f64-abab-5bb698c736dd.jpg', 
            'd028580f-9a98-4fb5-a6c9-5dc362ad3f09.jpg', '040d73b7-21b5-4cf2-84fc-e1a80231b202.jpg', '784d67d4-b95e-4abb-baf7-8024f18dc3c8.jpg']

data_1 = data_1.drop(data_1[data_1['image'].isin(corrupted)].index, axis=0)

# visualization
fig = px.bar(data_1['label'].value_counts())
fig.show()

class_names = list(data_1['label'].unique())
print(len(class_names))
class_names
class_dict = dict(zip(class_names, range(len(class_names))))
labels_to_remove = ['Skip', 'Not sure', 'Other', 'Blouse']
data_1 = data_1[~data_1['label'].isin(labels_to_remove)]
fig = px.bar(data_1['label'].value_counts())
fig.show()

y_train = data_1['label']

def show_random_training_image():
    random_index = random.choice(data_1.index)
    img_path = data_1['image'][random_index]
    label = data_1['label'][random_index]
    complete_path = TRAINING_DIR + '/' + img_path
    image = mpimg.imread(complete_path)
    plt.imshow(image)
    plt.axis('off')
    plt.title(label)

plt.figure(figsize=(20, 20))
for i in range(16):
    ax = plt.subplot(4, 4, i+1)
    show_random_training_image()

train_datagen = ImageDataGenerator(rescale=1./255,
                                  rotation_range=40,
                                  width_shift_range=0.3,
                                  height_shift_range=0.3,
                                  zoom_range=0.3,
                                  horizontal_flip=True,
                                  validation_split=0.1)

train_generator = train_datagen.flow_from_dataframe(
    dataframe=data_1,
    directory=TRAINING_DIR,
    x_col='image',
    y_col='label',
    target_size=IMAGE_SIZE,
    class_mode='categorical',
    batch_size=BATCH_SIZE,
    shuffle=True,
    subset='training'
)

validation_generator = train_datagen.flow_from_dataframe(
    dataframe=data_1,
    directory=TRAINING_DIR,
    x_col='image',
    y_col='label',
    target_size=IMAGE_SIZE,
    class_mode='categorical',
    batch_size=BATCH_SIZE,
    shuffle=False,
    subset='validation'
)

from tensorflow.keras.applications.mobilenet_v2 import MobileNetV2

pre_trained_model = MobileNetV2(input_shape=(224, 224, 3),
                                include_top=False,
                                weights='imagenet')

for layer in pre_trained_model.layers:
    layer.trainable = True

# pre_trained_model.summary()

# last_layer = pre_trained_model.get_layer('out_relu')
# print('last layer output shape: ', last_layer.output_shape)
# last_output = last_layer.output

# x = Flatten()(last_output)
# x = Dropout(0.3)(x)
# x = Dense(256, activation='relu', kernel_regularizer='l2')(x)
# x = Dropout(0.3)(x)
# x = Dense(16, activation='softmax')(x)           

# model = Model(pre_trained_model.input, x)

# learn_rate = LEARNING_RATE

# es = EarlyStopping(monitor='val_loss', mode='min', verbose=1, patience=10)
# cb = ModelCheckpoint('mobilenetv2.h5', save_best_only=True)
# lrr= ReduceLROnPlateau(monitor='val_accuracy', factor=.1, patience=5, min_lr=1e-5)

# adam = adam_v2.Adam(learning_rate=learn_rate)
# model.compile(loss='categorical_crossentropy',optimizer=adam, metrics=['accuracy'])


# history = model.fit(train_generator, epochs=EPOCHS,
#                     validation_data=validation_generator, 
#                     callbacks=[es, cb, lrr])

model = tf.keras.models.load_model('mobilenetv2.h5')

loss, accuracy = model.evaluate(validation_generator, verbose=0)

print(f"Model Loss is {loss:.2f} and Accuracy is {100*np.round(accuracy, 4)}%")

# def plot_loss_curves(history):
#     loss = history.history['loss']
#     val_loss = history.history['val_loss']

#     accuracy = history.history['accuracy']
#     val_accuracy = history.history['val_accuracy']

#     epochs = range(len(history.history['loss']))

#     # Plot loss
#     plt.plot(epochs, loss, label='training_loss')
#     plt.plot(epochs, val_loss, label='val_loss')
#     plt.title('Loss')
#     plt.xlabel('Epochs')
#     plt.legend()
    
#     # Plot accuracy
#     plt.figure()
#     plt.plot(epochs, accuracy, label='training_accuracy')
#     plt.plot(epochs, val_accuracy, label='val_accuracy')
#     plt.title('Accuracy')
#     plt.xlabel('Epochs')
#     plt.legend()
# plot_loss_curves(history)

classes = list(train_generator.class_indices.keys())

custom_dir = './input/clothing-dataset/images_compressed'

custom_paths = []
for image in os.listdir(custom_dir):
        custom_paths.append(custom_dir+'/'+image)

def predict_random_image():
    path = random.choice(custom_paths)
    image = tf.keras.preprocessing.image.load_img(path, target_size=(224, 224))
    input_arr = tf.keras.preprocessing.image.img_to_array(image)
    input_arr = np.array([input_arr])
    input_arr = input_arr.astype('float32') / 255.
    predictions = model.predict(input_arr, verbose=0)
    series = pd.Series(predictions[0], index=classes)
    predicted_classes = np.argsort(predictions)
    predictions.sort()
    plt.title(f"{classes[predicted_classes[0][-1]]} - {round(predictions[0][-1] * 100,2)}% \n{classes[predicted_classes[0][-2]]} - {round(predictions[0][-2] * 100,2)}% \n{classes[predicted_classes[0][-3]]} - {round(predictions[0][-2] * 100,3)}%")
    plt.imshow(image)
    plt.axis('off')

plt.figure(figsize=(20, 20))
for i in range(16):
    ax = plt.subplot(4, 4, i + 1)
    predict_random_image()