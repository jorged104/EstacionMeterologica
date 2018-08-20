import json
import flask
import MySQLdb
import time

DB_HOST = '35.225.49.177'
DB_USER = 'arqui'
DB_PASS = 'iotdatos'
DB_NAME = 'datos'
LATITUD = -8558
LONGITUD = 85885
app = flask.Flask(__name__)

def run_query(query=''):
    datos = [DB_HOST, DB_USER, DB_PASS, DB_NAME]

    conn = MySQLdb.connect(*datos)  #Abriendo Conexion
    cursor = conn.cursor()
    cursor.execute(query)

    if query.upper().startswith('SELECT'):
        data = cursor.fetchall()
    else:
        conn.commit()
        data = "Vacio"

    cursor.close()
    conn.close()
    return data

@app.route('/agregar')
def index():
    dato = ""
    #data = run_query("select * FROM Clima");
    dato = flask.request.args.get('datos')
    hora = time.strftime("%H:%M:%S")
    fecha = time.strftime("%Y-%m-%d")
    lista = dato.split(',')
    #insert INTO Clima(Fecha,Hora,Latitud,Longitud,Temperatura,Radiacion,Presion,Humedad)
    #values(DATE_FORMAT('01/08/18', "%d/%m/%y"),'21:00:00',5555,5555,23.00,25156156,810,49);
    sql = "insert INTO Clima(Fecha,Hora,Latitud,Longitud,Temperatura,Radiacion,Presion,Humedad)"
    sql += "values('"+str(fecha)+"',"
    sql +=  "'" + str(hora) + "'" + ","
    sql += str(LATITUD) + ","
    sql += str(LONGITUD) + ","
    sql += str(lista[1]) + ","
    sql += str(lista[3]) + ","
    sql += str(lista[2]) + ","
    sql += str(lista[0]) + ")"
    run_query(sql);
    return sql

@app.route('/getDatos')
def get():
    sql = "SELECT * FROM Clima";
    query = run_query(sql);
    return str(query);


@app.route('/holamundo')
def hello():
    return "Holista desde flask"
