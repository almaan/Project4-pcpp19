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
    a = x / 50.0 * cos(x*x / 100 ) * cos(x / 10)
    b = sin(x*x / 100) * sin( x / 10.0) * 0.1 
    return a - b

def dudyfun(x,y):
    return np.ones(y.shape)

def lapfun(x,y):
    a = (-10 * x * cos(x*x/100) * sin(x/10))
    b = cos(x/10) * (-50 * cos(x*x/100) + (25 + x**2) * sin(x*x/100))
    return ( a - b ) / 2500

def errest(v1,v2):
    return np.abs(v1-v2)


def plt_fun(ax,
            x,
            y,
            vals,
            mnmx = None,
            cmap = plt.cm.winter):

    vals[vals > 1e10] = 1e10
    if mnmx is None:
        mn = np.quantile(vals,0.05)
        mx = np.quantile(vals,0.95)
        mnmx = (mn,mx)

    ax.scatter(x = x,
               y= y,
               c = vals,
               s = 5,
               edgecolor = 'none',
               vmin = mnmx[0],
               vmax = mnmx[1],
               cmap = cmap,
               alpha = 0.3,
               )
    ax = clr_ax(ax)

    ax.set_aspect('equal')

    return ax

def clr_ax(ax):

    for sp in ax.spines.values():
        sp.set_visible(False)

    return ax



def plt_hist(ax,
             vals,
             edgecolor = 'black',
             facecolor = 'gray',
             ):

    ax.hist(vals,
            edgecolor = edgecolor,
            facecolor = facecolor,
            )

    ax.set_xticklabels(["{:0.2e}".format(x) for x in ax.get_xticks()],
                       rotation = 90,
                       fontsize = 5,
                       minor = False)

    ax = clr_ax(ax)
    return ax

def plt_compare(axarr,
                x,
                y,
                v1,
                v2,
                mnmx = None,
                ):

    assert axarr.shape[0] == 4


    axarr[0] = plt_fun(axarr[0],x,y,v1,mnmx)
    axarr[0].set_title("True Values")
    axarr[1] = plt_fun(axarr[1],x,y,v2,mnmx)
    axarr[1].set_title("Approximated Values")

    err = errest(v1,v2) 
    axarr[2] = plt_fun(axarr[2],x,y,err,cmap = plt.cm.Reds)
    axarr[2].set_title("Error")

    axarr[3] = plt_hist(axarr[3],err)
    axarr[3].set_title('Error Distribution')

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

        mx = np.max((np.quantile(u_true,0.95),np.quantile(u_aprx,0.95)))
        mx += np.abs(u_true.mean())
        mn = np.min((np.quantile(u_true,0.05),np.quantile(u_aprx,0.05)))
        mn -= np.abs(u_true.mean())

        fig,axr = plt.subplots(2,2,figsize=(5*2 + 0.2,4.5))
        axr = axr.flatten()

        axr = plt_compare(axr,
                        x,
                        y,
                        v1 = u_true,
                        v2 = u_aprx,
                        mnmx = (mn,mx),
                        )

        obase = name.replace('_','-') + '-compare.png'
        opth = osp.join(data['odir'],obase)
        fig.suptitle(name)
        fig.savefig(opth)
        plt.close("all")

    fig,axc = plt.subplots(1,1, figsize = (10 + 0.2,5))

    axc.scatter(x = x,
                y = y,
                c = "gray",
                edgecolor = 'black',
                s = 2,
                alpha = 0.8)
    axc = clr_ax(axc) 

    axc.set_aspect("equal")
    
    fig.suptitle("Grid")
    obase = "grid.png"
    opth = osp.join(data['odir'],obase)
    fig.savefig(opth)

    plt.close("all")



if __name__ == '__main__':
    main()
