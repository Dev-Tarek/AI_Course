import numpy as np
import pandas as pd

from sklearn.preprocessing import LabelEncoder
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
from sklearn.model_selection import train_test_split


data = pd.read_csv(r"Abalone\abalone.data", header=None)

le = LabelEncoder()
le.fit(data[data.columns[0]])
data[data.columns[0]] = le.transform(data[data.columns[0]])

X = data.iloc[:, 0:len(data.columns)-1] 
Y = data.iloc[:, len(data.columns)-1]   

x_train, x_test, y_train, y_test = train_test_split(X, Y, test_size=.3333, random_state=42)

model = LinearRegression()
model.fit(x_train, y_train)

result = model.predict(x_test)
error = np.sqrt(mean_squared_error(y_test, result))
print(error)