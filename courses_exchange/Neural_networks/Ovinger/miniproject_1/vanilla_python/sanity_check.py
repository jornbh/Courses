from initialize_miniproject_1 import *


model = Sequential([
    Dense( len(x_number_train[0]) ),
    (Activation('relu')),
    (Dense(y_number_train.shape[1])),
    (Activation("softmax")),
])


model.compile(
        loss="categorical_crossentropy",
        metrics=["acc"],
        optimizer=optimizers.SGD()
    )


history = model.fit( 
                                    x_number_train, 
                                    y_number_train,
                                    epochs=10,
                                    batch_size=128,
                                    validation_split=0.2
                                )

predictions = model.predict(x_number_test)
fig =plot_history(history, "Is this even sane")
print(  predictions)
fig.show()
plt.show()
print("DONE")