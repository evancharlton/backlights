import firebase from "firebase/app";
import "firebase/database";
import Monitor from "./Monitor";
import classes from "./App.module.css";

const firebaseConfig = {
  apiKey: "AIzaSyC0O5p-NdO6_sZEYd95tllY3LA-y7XukbE",
  authDomain: "aurora-backlights.firebaseapp.com",
  databaseURL:
    "https://aurora-backlights-default-rtdb.europe-west1.firebasedatabase.app",
  projectId: "aurora-backlights",
  storageBucket: "aurora-backlights.appspot.com",
  messagingSenderId: "576289851955",
  appId: "1:576289851955:web:c8af61db06cbfcfdc57146",
};

if (firebase.apps.length === 0) {
  firebase.initializeApp(firebaseConfig);
}

function App() {
  return (
    <div className={classes.container}>
      <Monitor firebasePath="/rgba/left" dimens={[1440, 2560]} />
      <Monitor firebasePath="/rgba/right" dimens={[2560, 1440]} />
    </div>
  );
}

export default App;
