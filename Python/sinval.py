# -*- coding: utf-8 -*-
"""
Created on Tue Sep 24 14:18:00 2013

@author: elenpp
"""
import numpy as np
import matplotlib.pylab as plt

val = np.sin(np.deg2rad(np.linspace(0, 360, 4000)))*0.2875*3333
#fmt_val_rnd = [np.round(v, 3) for v in val]
fmt_val_int = [int(v) for v in val]
fmt_val_int_bi20 = [v+1667 for v in fmt_val_int]
fmt_val_hex = [hex(v) for v in fmt_val_int]
plt.plot(fmt_val_int_bi20)