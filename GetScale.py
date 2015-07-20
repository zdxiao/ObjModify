# -*- coding: utf-8 -*-
"""
Created on Thu May 21 10:20:04 2015

@author: xzd
"""
from numpy import *

def read_labelFile(filename):
    file = open(filename, 'r')
    num = int(file.readline().strip())
    labels = zeros((num, 3))
    for i in range(num):
        line = file.readline().strip().split(' ')
        labels[i, :] = line[-3:];
    file.close();
    return labels
    
def calScale(refer_labels, target_labels):
    refer_mean = mean(refer_labels, 0)
    refer_center = refer_labels - refer_mean
    target_mean = mean(target_labels, 0)
    target_center = target_labels - target_mean
    refer_dis = sqrt(sum(sum(refer_center * refer_center, 1)))
    target_dis = sqrt(sum(sum(target_center * target_center, 1)))
    scale = refer_dis / target_dis
    return scale
    