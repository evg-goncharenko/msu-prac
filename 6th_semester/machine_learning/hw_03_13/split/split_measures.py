import numpy as np

def evaluate_measures(sample):
    """Calculate measure of split quality (each node separately).

    Parameters
    ----------
    sample : a list of integers. The size of the sample equals to the number of objects in the current node. The integer
    values are equal to the class labels of the objects in the node.

    Returns
    -------
    measures - a dictionary which contains three values of the split quality.
    Example of output:

    {
        'gini': 0.1,
        'entropy': 1.0,
        'error': 0.6
    }
    """
    sample = np.array(np.sort(sample))
    _, values = np.unique(sample, return_counts=True)
    new_sample = values / np.sum(values)
    res_gini = 1 - np.sum(new_sample ** 2)
    
    res_entropy = (-1) * np.sum(new_sample * np.log(new_sample))
    
    res_error = 1 - np.max(new_sample)
    measures = {'gini': float(res_gini), 'entropy': float(res_entropy), 'error': float(res_error)}
    return measures
