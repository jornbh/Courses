# Problems with sigmoidal

- The sigmoidal function has a derivatibe that is usually somewhat small when the input to the function becomes larger or smaller.

  - This is not a problem for shallow networks
  - But, when the network is deep, it can cause the gradient to become too small to become effective for the training set.
  - ReLu does not have this problem, and it is why ReLu is populair within the hidden layers.
  - (Residual networks are another solution, where you add the input directly to the output, and since the input is not "squashed", it reduces this problem)

- THe issue arises when a large input space is maped to a small one
- Batch normalisation reduces this by simply normalises the input, so that the derivative does not become too small. ![Plot of good areas to be in](https://cdn-images-1.medium.com/max/800/1*XCtAytGsbhRQnu-x7Ynr0Q.png)
