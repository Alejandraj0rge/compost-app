// Import the functions you need from the SDKs you need

import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
import { getFirestore } from "@firebase/firestore";


// TODO: Add SDKs for Firebase products that you want to use

// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration

// For Firebase JS SDK v7.20.0 and later, measurementId is optional

// import firebaseConfig from "./.env";

// Initialize Firebase

export const firebaseConfig = {

    apiKey: "AIzaSyCgN_QWizzfjrioM-LANdO4iNdAxSPfz6o",

    authDomain: "compost-project.firebaseapp.com",

    projectId: "compost-project",

    storageBucket: "compost-project.appspot.com",

    messagingSenderId: "840238358831",

    appId: "1:840238358831:web:9295dab28b3d7b5d5dc1da",

    measurementId: "G-Y79Y0TWTBR"

};

const app = initializeApp(firebaseConfig);

export const db = getFirestore(app);

const analytics = getAnalytics(app);