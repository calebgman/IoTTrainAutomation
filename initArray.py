import pandas as pd

data = pd.DataFrame(
    {"ID" : "TestyBlock",
    "TYPE" : "Default",
    "BLOCKING" : 0,
    "SIG_STATE" : 0,
    "OBSTACLE" : 0,
    "SW_STATE" : 0,
    "SPEED" : 0,
    "STOPPED" : 0,
    "CURR_LOC" : 0,
    "PREV_LOC" : 0,
    "PREV_PREV_LOC": 0},
    index = ["0000"])

print(data.head())

data.to_csv("nodes.csv", index=False)


