# This was made automatically, so I dumped ecerything in task 1

# Everything needed to run task 4 and later (This keeps jupyter notebook from hogging all the memomy, and killing the PC)

student1 = "Jørn Bøni Hofstad"
student2 = "Pleis H. Older"

import numpy as np
import time
import matplotlib.pyplot as plt
import scipy.io

import keras
from keras.models import Sequential
from keras.layers import Dense, Conv2D, MaxPooling2D, Dropout, Flatten
from keras.optimizers import SGD, Adam

# MY stuff 
GLOBAL_N_EPOCHS = 50 # Controll variable for debugging the program
NELDER_MEAD_ITTERATIONS = 20
DEBUG_BATCH_SIZE = 128
import keras
from keras.models import Sequential
from keras.layers import Dense, Conv2D, MaxPooling2D, Dropout, Flatten
from keras.optimizers import SGD, Adam



# Handed out functionaliry
################################################################################
# Ploting functionality
def plot_some_samples(x, y = [], yhat = [], select_from = [], 
                      ncols = 6, nrows = 4, xdim = 28, ydim = 28,
                      label_mapping = range(10)):
    """plot some input vectors as grayscale images (optionally together with their assigned or predicted labels).
    
    x is an NxD - dimensional array, where D is the length of an input vector and N is the number of samples.
    Out of the N samples, ncols x nrows indices are randomly selected from the list select_from (if it is empty, select_from becomes range(N)).
    
    Keyword arguments:
    y             -- corresponding labels to plot in green below each image.
    yhat          -- corresponding predicted labels to plot in red below each image.
    select_from   -- list of indices from which to select the images.
    ncols, nrows  -- number of columns and rows to plot.
    xdim, ydim    -- number of pixels of the images in x- and y-direction.
    label_mapping -- map labels to digits.
    
    """
    fig, ax = plt.subplots(nrows, ncols)
    if len(select_from) == 0:
        select_from = range(x.shape[0])
    indices = np.random.choice(select_from, size = min(ncols * nrows, len(select_from)), replace = False)
    for i, ind in enumerate(indices):
        thisax = ax[i//ncols,i%ncols]
        thisax.matshow(x[ind].reshape(xdim, ydim), cmap='gray')
        thisax.set_axis_off()
        if len(y) != 0:
            j = y[ind] if type(y[ind]) != np.ndarray else y[ind].argmax()
            thisax.text(0, 0, (label_mapping[j]+1)%10, color='green', 
                                                       verticalalignment='top',
                                                       transform=thisax.transAxes)
        if len(yhat) != 0:
            k = yhat[ind] if type(yhat[ind]) != np.ndarray else yhat[ind].argmax()
            thisax.text(1, 0, (label_mapping[k]+1)%10, color='red',
                                             verticalalignment='top',
                                             horizontalalignment='right',
                                             transform=thisax.transAxes)
    return fig
def prepare_standardplot(title, xlabel):
    fig, (ax1, ax2) = plt.subplots(1, 2)
    fig.suptitle(title)
    ax1.set_ylabel('categorical cross entropy')
    ax1.set_xlabel(xlabel)
    ax1.set_yscale('log')
    ax2.set_ylabel('accuracy [% correct]')
    ax2.set_xlabel(xlabel)
    return fig, ax1, ax2
def finalize_standardplot(fig, ax1, ax2):
    ax1handles, ax1labels = ax1.get_legend_handles_labels()
    if len(ax1labels) > 0:
        ax1.legend(ax1handles, ax1labels)
    ax2handles, ax2labels = ax2.get_legend_handles_labels()
    if len(ax2labels) > 0:
        ax2.legend(ax2handles, ax2labels)
    fig.tight_layout()
    plt.subplots_adjust(top=0.9)
def plot_history(history, title):
    fig, ax1, ax2 = prepare_standardplot(title, 'epoch')
    ax1.plot(history.history['loss'], label = "training")
    ax1.plot(history.history['val_loss'], label = "validation")
    ax2.plot(history.history['acc'], label = "training")
    ax2.plot(history.history['val_acc'], label = "validation")
    finalize_standardplot(fig, ax1, ax2)
    return fig


# This plotting routine might help you ...
def comparison_plot(history_sgd, history_adam, label1, label2, title):
    fig, ax1, ax2 = prepare_standardplot(title, "epochs")
    ax1.plot(history_sgd.history['loss'], label=label1 + ' training')
    ax1.plot(history_sgd.history['val_loss'], label=label1 + ' validation')
    ax1.plot(history_adam.history['loss'], label=label2 + ' training')
    ax1.plot(history_adam.history['val_loss'], label=label2 + ' validation')
    ax2.plot(history_sgd.history['acc'], label=label1 + ' training')
    ax2.plot(history_sgd.history['val_acc'], label=label1 + ' validation')
    ax2.plot(history_adam.history['acc'], label=label2 + ' training')
    ax2.plot(history_adam.history['val_acc'], label=label2 + ' validation')
    finalize_standardplot(fig, ax1, ax2)
    return fig


# My owns stuff again
################################################################################



#Creating my own datasets 
########################################
#Datasets
from keras.datasets import mnist
from keras.datasets import fashion_mnist

(x_number_train_raw, y_number_train_raw), (x_number_test_raw, y_number_test_raw) = mnist.load_data()
(x_fashion_train_raw, y_fashion_train_raw), (x_fashion_test_raw, y_fashion_test_raw) = fashion_mnist.load_data()

x_number_train = x_number_train_raw.reshape(x_number_train_raw.shape[0], x_number_train_raw.shape[1]*x_number_train_raw.shape[2])/np.max(x_number_train_raw)
x_number_test = x_number_test_raw.reshape(x_number_test_raw.shape[0], x_number_test_raw.shape[1]*x_number_test_raw.shape[2])/np.max(x_number_test_raw)

x_fashion_train = x_fashion_train_raw.reshape(x_fashion_train_raw.shape[0], x_fashion_train_raw.shape[1]*x_fashion_train_raw.shape[2])/np.max(x_fashion_train_raw)
x_fashion_test = x_fashion_test_raw.reshape(x_fashion_test_raw.shape[0], x_fashion_test_raw.shape[1]*x_fashion_test_raw.shape[2])/np.max(x_fashion_test_raw)

y_number_train = keras.utils.to_categorical(y_number_train_raw)
y_test = keras.utils.to_categorical(y_number_test_raw)

y_fashion_train = keras.utils.to_categorical(y_fashion_train_raw)
y_fashion_test = keras.utils.to_categorical(y_fashion_test_raw)

x_number_train = x_number_train_raw.reshape(x_number_train_raw.shape[0], x_number_train_raw.shape[1]*x_number_train_raw.shape[2])/np.max(x_number_train_raw)

x_number_test = x_number_test_raw.reshape(x_number_test_raw.shape[0], x_number_test_raw.shape[1]*x_number_test_raw.shape[2])/np.max(x_number_test_raw)

x_fashion_train = x_fashion_train_raw.reshape(x_fashion_train_raw.shape[0], x_fashion_train_raw.shape[1]*x_fashion_train_raw.shape[2])/np.max(x_fashion_train_raw)
x_fashion_test = x_fashion_test_raw.reshape(x_fashion_test_raw.shape[0], x_fashion_test_raw.shape[1]*x_fashion_test_raw.shape[2])/np.max(x_fashion_test_raw)

# Libraries for making the neural networks
########################################
from keras.models import Sequential
from keras.layers import Dense, Activation
from keras import optimizers


# Function that will plot something /somewhere/, both in jupyter notebook, if run normally in the project
########################################
def reckless_savefig(fig, filename):
    try: 
        fig.savefig(filename)
        plt.close(fig)
    except: 
        print("Unable to save plot with filename:", filename )
        fig.show()

