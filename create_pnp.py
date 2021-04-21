#!/usr/bin/python

import pandas as pd

def fetch_pnp(designators, file, side, output):
    df = pd.read_csv(file, header=None);
    for row_index,row in df.iterrows():
        for item in designators:
            if (row[0] == item):
                values = {'Designator': row[0],
                          'Footprint' : row[5],
                          'PosX'      : row[1],
                          'PosY'      : row[2],
                          'Side'      : side,
                          'Rotation'  : row[3]}
                #print(values)
                new_row = pd.Series(values, name=row[0]);
                output = output.append(new_row, ignore_index=False);
    #print(df);
    return output

def fetch_bom(file, pnp):
    """
    Item,Name,Qty,Manufacturer,Mfg Part #,Descriptor/Value,Package/Footprint,Instructions
    """
    df = pd.read_csv(file);
    count = 0
    new_count = 0
    for row_index,row in df.iterrows():
        ref = list(row[1].split(','))
        #print(ref)
        new_count += len(ref)
        count += row[2]

        if row[2] != len(ref):
            print("BOM vs PnP: Item ",row[1]," has incorrect number. We found ", len(ref), " with spreadsheet having ", row[2])
        found = 0
        for name in ref:
            #print(name)
            for i,data in pnp.iterrows():
                #print(row[0])
                if data[0] == name:
                    found += 1
                    break;

        if found != len(ref):
            print("BOM vs PnP: One of the items ", row[1]," is missing from PnP");
            return 1

    print("BOM vs PnP: Count of items=",count," calculated=",new_count," OK!");
    return 0

def fetch_data():
    designators = []
    fp = open("designator.txt",'r');
    lines = fp.readlines();
    count = 0;
    for line in lines:
        designators.append(line.strip())
    fp.close()
    return designators

l = fetch_data()
#print(l)
# Designator Footprint PosX PosY Side Rot
output =  pd.DataFrame(columns=['Designator','Footprint','PosX','PosY','Side','Rotation'])
# R8  24.64  44.45  270.0         100K                           1206

version="v1.4-rc5"

back_str="%s/CAMOutputs/Assembly/PnP_Wash_Hand_back.csv" % (version)
front_str="%s/CAMOutputs/Assembly/PnP_Wash_Hand_front.csv" % (version)

output = fetch_pnp(l,back_str,'Bottom', output)
output = fetch_pnp(l,front_str,'Top',output)

pnp_str="%s/PnP.csv" % (version)
bom_str="%s/BOM.csv" % (version)

output.to_csv(pnp_str, index=False)
fetch_bom(bom_str, output)
