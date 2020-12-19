# Task 3 awnsers

**Question**: What happens if the learning rate of SGD is A) very large B) very small? Please answer A) and B) with one full sentence each (double click this markdown cell to edit).

**Answer**:

A) The range 0.01 to o.1, the stocastic gradiet descent converges more slowly when the learning rate is low. As the learning-rate increases, the accuracy of the training set increases more quickly. We also start seeing that the distance between the trainging set and the verification set becomes more noticeable as the learning rate increases.

B) The Adams method has the advantage when the learning rate is very low, but it ends up performing more poorly when learnign rate is around 0.02. In addition, in these small examples, the asymptotic value of the adams method also seems to be noticeably worse tha

**Question**: At which epoch (approximately) does the Adam optimizer start to overfit (on MNIST)? Please answer with one full sentence.

**Answer**: From the pictures, it can be seen that the adams method starts overfiting roughly around epoch 4

**Question**: Explain the qualitative difference between the loss curves and the accuracy curves with respect to signs of overfitting. Please answer with at most 3 full sentences.


**Answer**: The accuracy function is based on slimple true or false for weterer or not the value that was guessed was correct or not. Loss, on the other hand is also affected by how confidently the model states something, giving a lower loss if the model is very confident on all the correct statements, but also taking a seere hit if any of the estimates it was so confident in proves to be wrong. So, if the loss decreases significantly, without any significant change in the accuracty, it is most likely a sign of that the model is overfiting. 