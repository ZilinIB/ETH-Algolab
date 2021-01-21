# Idea

find the minimal covering circle.

# Remark
The running time of ifferent kernels vary, sometimes we may need to use multiple kernels for different subtasks to accelerate the program.

In this case, we can compute the min circle using the EPECK kernel, extract the supporting points and then constuct another min circle with the sqrt kernel. Then calculating the radius in the new kernel.