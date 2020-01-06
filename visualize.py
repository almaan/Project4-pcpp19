#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from scipy.interpolate import griddata

import yaml

import os.path as osp
import sys

sin = np.sin
cos = np.cos

def ufun(x,y):
    return sin(x**2/100)*cos(x/10) + y

def dudxfun(x,y):
    a = (x/5.0 * np.cos(x**2/100)*cos(x/10.0))
    b = (sin(x**2/100))*sin(x/10)
    return 0.1 * ( a - b )

def dudyfun(x,y):
    return np.ones(y.shape)

def lapfun(x,y):
    a = (-10 * x * cos(x**2/100) * sin(x/10))
    b = -cos(x/10) * (-50 * cos(x**2/100) + (25 + x**2) * sin(x**2/100))
    return ( a + b ) / 2500

def errest(v1,v2):
    return np.abs(v1-v2)


def plt_fun(ax,
            x,
            y,
            vals,
            mnmx = None,
            cmap = plt.cm.magma):

    if mnmx is None:
        mnmx = (vals.min(),vals.max())

    ax.scatter(x = x,
               y= y,
               c = vals,
               s = 80,
               vmin = mnmx[0],
               vmax = mnmx[1],
               cmap = plt.cm.magma,
               )

    ax.set_aspect('equal')

    for sp in ax.spines.values():
        sp.set_visible(False)

    return ax

def plt_compare(axarr,
                x,
                y,
                v1,
                v2,
                mnmx):

    assert axarr.shape[0] == 3

    axarr[0] = plt_fun(axarr[0],x,y,v1,mnmx)
    axarr[0].set_title("True Values")
    axarr[1] = plt_fun(axarr[1],x,y,v2,mnmx)
    axarr[1].set_title("Approximated Values")

    err = errest(v1,v2)
    axarr[2] = plt_fun(axarr[2],x,y,err,cmap = plt.cm.PuRd)
    axarr[2].set_title("Error")

    return axarr


def main():

    input_pth = sys.argv[1]

    with open(input_pth) as fopen:
        data_spec = yaml.safe_load(fopen)

    funmap = {'partial_x' : dudxfun,
              'partial_y':dudyfun,
              'laplacian':lapfun,
              'fun':ufun,
              }

    for name,data in data_spec.items():

        x = np.fromfile(data['x'])
        y = np.fromfile(data['y'])

        u_true = funmap[data['fun']](x,y)
        u_aprx = np.fromfile(data['vals'])

        mx = np.max((u_true.max(),u_aprx.max()))
        mn = np.min((u_true.min(),u_aprx.min()))

        fig,axr = plt.subplots(1,3,figsize=(5*3 + 0.2,2))

        axr = plt_compare(axr,
                        x,
                        y,
                        v1 = u_true,
                        v2 = u_aprx,
                        mnmx = (mn,mx),
                        )

        obase = name + '-compare.png'
        opth = osp.join(data['odir'],obase)

        print(opth)
        fig.suptitle(name)
        fig.savefig(opth)


        plt.close("all")

if __name__ == '__main__':
    main()
