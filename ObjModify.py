# -*- coding: utf-8 -*-
"""
Created on Wed Apr 15 08:29:49 2015

@author: xzd
"""

from numpy import *
import random
import os

#filename = 'test.obj'
def file2matrix(filename):
    MaxInfoLine = 20
    verticesNum = 0
    trianglesNum = 0
    fr = open(filename)
    arrayOLines = fr.readlines()
    
    for i in range(MaxInfoLine):
        line = arrayOLines[i].strip()
        info = line.split(' ')
        if(len(info) > 2):
            if info[2] == 'vertices':
                verticesNum = int(info[1])
            elif info[2] in ('triangles', 'trianglar'):
                trianglesNum = int(info[1])
            else:
                pass
        else: 
            pass
    
    verticesMat = zeros((verticesNum, 3))
    trianglesMat = zeros((trianglesNum,3))
    
    v_index = 0
    t_index = 0
    for line in arrayOLines:
        line  = line.strip()
        listFromLine = line.split(' ')
        if listFromLine[0] == 'v':
            verticesMat[v_index, :] = listFromLine[1:4]
            v_index += 1
        elif listFromLine[0] == 'f':
            trianglesMat[t_index, :] = listFromLine[1:4]
            t_index += 1
    fr.close();
    return verticesMat, trianglesMat
    
def file2matrix_noInfo(filename):
    fr = open(filename)
    arrayOLines = fr.readlines()
    
    verticesArr = []
    trianglesArr = []
    dim = 3    
    
    for line in arrayOLines:
        lineArr = []
        listFromLine  = line.strip().split(' ')
        if listFromLine[0] == 'v':
            for i in range(dim):
                lineArr.append(float(listFromLine[i+1]))
            verticesArr.append(lineArr)
        elif listFromLine[0] == 'f':
            for i in range(dim):
                lineArr.append(int(listFromLine[i+1]))
            trianglesArr.append(lineArr)
    fr.close();
    return mat(verticesArr), mat(trianglesArr)
    
def pca(dataMat, topNfeat = 9999999):
    meanVals = mean(dataMat, axis = 0)
    meanRemoved = dataMat - meanVals
    covMat = cov(meanRemoved, rowvar = 0)
    eigVals, eigVects = linalg.eig(mat(covMat))
    eigValInd = argsort(eigVals)
    eigValInd = eigValInd[:-(topNfeat+1):-1]
    redEigVects = eigVects[:, eigValInd]
    lowDDataMat = meanRemoved * redEigVects
    reconMat = (lowDDataMat * redEigVects.T) + meanVals
    return lowDDataMat, reconMat

def judgeY(vMat):
    r = 50 # 50mm
    counter = 0
    ygt0 = 0
    num = 10
    for i in range(vMat.shape[0]):
        # if x and z near 0, in a range of 5cm circle
        if abs(vMat[i,0]) < r and abs(vMat[i,1]) < r: 
            counter = counter + 1
            if vMat[i,2] > 0: # if y gt 0
                ygt0 = ygt0 + 1
            if counter == num:
                break
    print "Y positive direction number " + str(ygt0)
    if ygt0 < num/2:
        vMat[:,2] = -vMat[:,2] # change Y direction
        vMat[:,1] = -vMat[:,1] # as Y changes, so do X
    return vMat
    
    
def matrix2file_afterPCA(filename, vMat, tMat, flag = True, factor = 1):
    # file header
    f = open(filename, 'w')
    f.write("###\n")
    f.write("### Beijing Normal University 3D Face Dataset -- May, 2015\n")
    f.write("### Contact Email:zd.xiao@qq.com, bfy122@hotmail.com\n")
    f.write("### obj file only contain v and f\n")
    f.write("# original sacal = mm, factor = " + str(factor) + "\n")
    f.write("# " + str(vMat.shape[0]) + " vertices\n")
    f.write("# " + str(tMat.shape[0]) + " triangles\n")
    f.write("###XZD###\n")
    #vertices 
    for i in range(vMat.shape[0]):
        f.write("v " + str(1 * vMat[i, 1] * factor)) # x
        f.write(" "  + str(1 * vMat[i, 2] * factor)) # y
        f.write(" "  + str(1 * vMat[i, 0] * factor) + "\n") # z
    #triangles
    if(flag):
        for i in range(tMat.shape[0]):
            f.write("f " + str(int(tMat[i, 0])))
            f.write(" "  + str(int(tMat[i, 1])))
            f.write(" "  + str(int(tMat[i, 2])))
            if(i != tMat.shape[0] - 1):
                f.write('\n')
    else:
        for i in range(tMat.shape[0]):
            f.write("f " + str(int(tMat[i, 0])))
            f.write(" "  + str(int(tMat[i, 2])))
            f.write(" "  + str(int(tMat[i, 1])))
            if(i != tMat.shape[0] - 1):
                f.write('\n')

def matrix2file(filename, vMat, tMat, factor = 0.006):
    # file header
    f = open(filename, 'w')
    f.write("###\n")
    f.write("### Beijing Normal University 3D Face Dataset -- May, 2015\n")
    f.write("### Contact Email:zd.xiao@qq.com, bfy122@hotmail.com\n")
    f.write("### obj file only contain v and f\n")
    f.write("# original sacal = mm, factor = " + str(factor) + "\n")
    f.write("# " + str(vMat.shape[0]) + " vertices\n")
    f.write("# " + str(tMat.shape[0]) + " triangles\n")
    f.write("###\n")
    #vertices 
    for i in range(vMat.shape[0]):
        f.write("v " + str(1 * vMat[i, 0])) # x
        f.write(" "  + str(1 * vMat[i, 1])) # y
        f.write(" "  + str(1 * vMat[i, 2]) + "\n") # z
    #triangles
    for i in range(tMat.shape[0]):
        f.write("f " + str(int(tMat[i, 0])))
        f.write(" "  + str(int(tMat[i, 1])))
        f.write(" "  + str(int(tMat[i, 2])))
        if(i != tMat.shape[0] - 1):
            f.write('\n')
                
def transform(lowDMat, tMat):
    randNum = 10
    randF = random.sample(xrange(tMat.shape[0]), randNum)
    count = 0
    for f in randF:
        v1 = int(tMat[f, 0]) - 1 # get vertices index
        v2 = int(tMat[f, 1]) - 1
        v3 = int(tMat[f, 2]) - 1
        vec2_1 = lowDMat[v2, :] - lowDMat[v1, :]
        vec3_2 = lowDMat[v3, :] - lowDMat[v2, :]
        f_nor = cross(vec2_1, vec3_2)
        val = f_nor * transpose(lowDMat[v1, :])
        # print val
        if(val > 0):
            count = count + 1
    print "Normal positive direction number " + str(count)
    if(count > randNum/2):
        return True
    else:
        return False

def newname(filename):
    newname = filename[0:-4] + "_pca.obj"
    return newname
    
def newname_formate(filename):
    newname = filename[0:-4] + "_fmt.obj"
    return newname
    
def normalize(filename, inPath = ".", outPath = ".", changeZ = 0):
    infile = inPath + "\\" + filename;
    outfile = newname(outPath + "\\" + filename);
    # input data
    vMat, tMat = file2matrix_noInfo(infile)
    # PCA
    lowDMat, reconMat = pca(vMat)
    # judge Y direction
    lowDMat = judgeY(lowDMat)
    # judge normal direction
    flag = transform(lowDMat, tMat)
    # change Z
    if changeZ:
        lowDMat[:,0] = -lowDMat[:,0] # change Z
        lowDMat[:,1] = -lowDMat[:,1] # change X as change Z
    # ouput data
    matrix2file_afterPCA(outfile, lowDMat, tMat, flag, factor = 0.004)
    
def batchNormalize(listFile, inPath = ".", outPath = "."):
    f = open(inPath + "\\" + listFile)
    lines = f.readlines()
    for line in lines:
        line = line.strip()
        print line
        normalize(line, inPath, outPath)
    
def transformFormate(filename, inPath = ".", outPath = "." , scale = 1):
    infile = inPath + "\\" + filename;
    outfile = newname_formate(outPath + "\\" + filename);
    # input data
    vMat, tMat = file2matrix_noInfo(infile)
    # Scale
    vMat = scale * vMat;
    # ouput data
    matrix2file(outfile, vMat, tMat)
    
def BatchTransform(listFile, inPath = ".", scale = 1):
    f = open(inPath + '\\' + listFile)
    liePath = inPath + '\\' + "Lie"
    isExists = os.path.exists(liePath)
    if not isExists:
        print liePath + " OK"
        os.makedirs(liePath)
    else:
        print liePath + " Exists"
    
    sitNPath = inPath + '\\' + "SitN"
    isExists = os.path.exists(sitNPath)
    if not isExists:
        print sitNPath + " OK"
        os.makedirs(sitNPath)
    else:
        print sitNPath + " Exists"
        
    sitSPath = inPath + '\\' + "SitS"
    isExists = os.path.exists(sitSPath)
    if not isExists:
        print sitSPath + " OK"
        os.makedirs(sitSPath)
    else:
        print sitSPath + " Exists"
        
    
    lines = f.readlines();
    for line in lines:
        line = line.strip()
        print line
        if line[-9:] == "n_lie.obj":
            outPath = liePath
        elif line[-9:] == "n_sit.obj":
            outPath = sitNPath
        elif line[-9:] == "s_sit.obj":
            outPath = sitSPath
        else:
            pass
        transformFormate(line, inPath, outPath, scale)
        
