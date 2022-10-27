import React, { useEffect, useState } from 'react';
import {db} from './firebaseConfig';
import {collection, getDocs/* , addDoc, setDoc, updateDoc, deleteDoc, doc */} from "firebase/firestore";
import { Stars, OrbitControls/* , Cloud, Text3D */} from '@react-three/drei'
import { Canvas} from '@react-three/fiber';
import './App.css';

function Box() {
	return (
		<mesh>
			<boxBufferGeometry attach="geometry"/>
			<meshLambertMaterial attach="material" color="hotpink"/>
		</mesh>
	)
}
 
function App() {

	const [temperatures, setTemperatures] =  useState([]);
	
 	useEffect(() => {
		const getTemperature = async () => {

			try {
				var variablesDoc = await getDocs(collection(db, "compost_variables"));
				var sensorDoc = await getDocs(collection(db, "sensor_data"));

				var sensorData = [];

				sensorDoc.forEach((doc) => {
					sensorData = sensorData.concat(doc.data());
				});

				console.log(sensorData);

				var data = [];

				variablesDoc.forEach((doc) => {
					var docData = doc.data();
					docData.value = sensorData[0][docData.variable]
					data = data.concat(docData);
				}); 

				console.log(data);

				setTemperatures(data);
				
			} catch (e) {
				console.error("Error adding document: ", e);
			}
		}

		getTemperature();
	}, []) 

	return (
		<div className="App"/*  style="display:flex;" */>
			<div id="myNav" className="overlay">
				<a href="" className="closebtn" /* onClick="closeNav()" */>&times;</a>
				<div className="overlay-content">
					<a id='welcome-title' href="">Welcome to compost project</a>
				</div> 
			</div>
			<div id='animation'>
				<Canvas>
	 	 			<OrbitControls/>
					<Stars/>
					<ambientLight intensity={0.5}/>
					<spotLight position={[10, 15, 10]} angle={0.3}/> 
					<Box/> 
				</Canvas>
				{/* <img src="compost-app\src\photo1665517962.jpeg" alt="Girl in a jacket" width="500" height="600"></img> */}
			</div>
			<div id='variables-form'>
				<div>
					{temperatures.map((temperature) => 
						<div id='variable-content'><div id='variable-name'>{temperature.label}:</div> <div id='variable-value'>{temperature.value}</div></div>)}
				</div>
			</div>
			<div id='column-3'>
				<div id='wiki-fact'>random wiki</div>
				{/* <div id='sim-btn'>follow a sim</div> */}
			</div>
		</div>
	);
}

export default App;
