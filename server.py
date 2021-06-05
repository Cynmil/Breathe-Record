from flask import Flask, render_template, request
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///sensordb.db'

db = SQLAlchemy(app)

# Create db models
class Info(db.Model):
	id = db.Column(db.Integer, primary_key=True)
	temp = db.Column(db.Float)
	hum = db.Column(db.Float)
	date = db.Column(db.DateTime, default=datetime.utcnow)

	def __repr__(self):
		return '<Name %r>' % self.id

@app.route("/", methods = ['GET'])
def home():
	temps = db.session.query(Info.temp).order_by(Info.date)
	hums = db.session.query(Info.hum).order_by(Info.date)
	dates = db.session.query(Info.date).order_by(Info.date)
	return render_template('index.html', temps=temps, hums=hums, dates=dates)

@app.route("/api", methods = ['POST', 'GET'])
def api():
	if request.method == "POST":
		content = request.json
		temp = float(content["temp"])
		hum = float(content["hum"])

		if (temp == 0 and hum == 0):
			print("Couldn't read the temperature or humidity this time.")
			return "Received invalid values"
		else:
			print("Temperature:", temp, "Celcius")
			print("Humidity:", hum)
			info = Info(temp=temp, hum=hum)

			try:
				db.session.add(info)
				db.session.commit()
				return "Successfully received a value and inserted into the database"
			except:
				return "Failed to add to the database"
	
	return "API"

@app.route("/api/data", methods = ['GET'])
def data():
	temps = []
	for row in db.session.query(Info.temp).order_by(Info.date).all():
		temps.append(row[0])
		
	hums = []
	for row in db.session.query(Info.hum).order_by(Info.date).all():
		hums.append(row[0])

	dates = []
	for row in db.session.query(Info.date).order_by(Info.date).all():
		dates.append(row[0])
		
	return {"temps": temps, "hums": hums, "dates": dates}

@app.route("/api/reset", methods = ['POST', 'GET'])
def reset():
	if request.method == 'POST':
		try:
			db.session.query(Info).delete()
			db.session.commit()
		except:
			db.session.rollback()
	return "RESET"
