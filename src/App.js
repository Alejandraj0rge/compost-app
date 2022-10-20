import { useEffect, useState } from 'react';
import {db} from './firebaseConfig';
import {collection, getDocs, addDoc, setDoc, updateDoc, deleteDoc, doc} from "firebase/firestore";
import './App.css';

function App() {
	const [temperatures, setTemperatures] =  useState([]);
	const usersCollectionRef = collection(db, "temperatures");
	
 	useEffect(() => {
		const getTemperature = async () => {

			var data = [
				{
					label: 'Temperature',
					type: 'celsius',
					variable: 'temperature'
				},
				{
					label: 'Humidity',
					type: 'percentage',
					variable: 'humidity'
				},
				{
					label: 'CO2',
					type: 'ppm',
					variable: 'co2'
				},
				{
					label: 'pH',
					type: 'integer',
					variable: 'ph'
				}
			];
			try {
				const washingtonRef = collection(db, "compost_variables");
/* 				const docRef = await addDoc(washingtonRef, {
					label: 'Temperature',
					type: 'celsius',
					variable: 'temperature'
				});  */
				var docRef = await getDocs(collection(db, "compost_variables"));
				var data = [];
		 		docRef.forEach((doc) => {
					console.log(doc.data());
					data = data.concat(doc.data());
				}); 
				setTemperatures(data);

				console.log(temperatures);
			} catch (e) {
				console.error("Error adding document: ", e);
			}
		}

		getTemperature();
	}, []) 

	return (
		<div className="App"/*  style="display:flex;" */>
			<div id="myNav" className="overlay">
				<a href="javascript:void(0)" className="closebtn" /* onClick="closeNav()" */>&times;</a>
				<div className="overlay-content">
					<a id='welcome-title' href="#">Welcome to compost project</a>
				</div> 
			</div>
			<div id='animation'>
				animation 
			</div>
			<div id='variables-form'>
				<div>
					<div id='variable-content'><div id='variable-name'>stage:</div> <div id='variable-value'>mesofile</div></div>
					<div><div>temperature:</div> <div>35*</div></div>
					<div><div>humidity:</div><div>20%</div></div>
					<div><div>CO2:</div> <div>5.3</div></div>
					<div><div>ph:</div> <div>5.3</div></div>
				</div>
			</div>
			<div id='column-3'>
				<div id='wiki-fact'>random wiki</div>
				<div id='sim-btn'>follow a sim</div>
			</div>
		</div>
	);
}

export default App;
