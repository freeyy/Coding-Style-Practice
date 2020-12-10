# Author: Coding Style Practice
# Modify: FReeYY

from keras.models import Sequential
from keras.layers import Dense
import numpy as np
import sys, os, string, random

characters = string.printable
char_indices = dict((c, i) for i, c in enumerate(characters))

leetcodes = ['4', '8', '<', '|)', '3', '|=', '[,', '#', '1', '_|', '|<', '|_', '44', '|\\|', '0', '|o', 'O_', '|2', '5', '7', '|_|', '\\/', 'VV', '%', '`/', '2']
indices_leet = dict((i, c) for i, c in enumerate(leetcodes))
indices_leet[30] = ' '

INPUT_VOCAB_SIZE = len(characters)
BATCH_SIZE = 200

def encode_one_hot(line):
    x = np.zeros((len(line), INPUT_VOCAB_SIZE))
    for i, c in enumerate(line):
        if c in characters:
            index = char_indices[c]
        else:
            index = char_indices[' ']
        x[i][index] = 1 
    return x

def decode_one_hot(x):
    s = []
    for onehot in x:
        one_index = np.argmax(onehot) 
        s.append(indices_leet[one_index]) 
    return ''.join(s)

def onehot_index_char(c):
    if c in characters:
        index = char_indices[c]
    else:
        index = char_indices[' ']    
    return index

def onehot_index_leet(c):
    if c in string.ascii_letters:
        index = ord(c.lower()) - ord('a')
    else:
        index = 30
    return index
    
def build_model():
    # Normalize characters using a dense layer
    model = Sequential()
    dense_layer = Dense(INPUT_VOCAB_SIZE, 
                        input_shape=(INPUT_VOCAB_SIZE,),
                        activation='softmax')
    model.add(dense_layer)
    return model

def input_generator(nsamples):
    def generate_line():
        x = np.zeros((nsamples, INPUT_VOCAB_SIZE))
        y = np.zeros((nsamples, INPUT_VOCAB_SIZE))
        for i in range(nsamples):
            c = random.choice(characters)
            x[i][onehot_index_char(c)] = 1
            y[i][onehot_index_leet(c)] = 1
        return x, y
    
    while True:
        yield generate_line()

def train(model):
    model.compile(loss='categorical_crossentropy',
                  optimizer='adam',
                  metrics=['accuracy'])
    input_gen = input_generator(BATCH_SIZE)
    validation_gen = input_generator(BATCH_SIZE)
    model.fit_generator(input_gen,
                epochs = 50, workers=1,
                steps_per_epoch = 20,
                validation_data = validation_gen,
                validation_steps = 10)

model = build_model()
model.summary()
train(model)

input("Network has been trained. Press <Enter> to run program.")

with open(sys.argv[1]) as f:
    for line in f:
        if line.isspace(): continue
        batch = encode_one_hot(line)
        preds = model.predict(batch)
        leet = decode_one_hot(preds)
        print(leet)