package demo;

import java.io.FileWriter;
import java.io.IOException;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

//Create a JSON object, marshal it, and store it in a file.
public class Marshaling {
	
	@SuppressWarnings("unchecked")
	public static void main(String[] args) {
		
		//JSON object representing a student with the following properties: ID, name, isEnrolled, subjects
		JSONObject student = new JSONObject();
		student.put("ID", 1);
		student.put("name", "Irum Bukhari");
		student.put("isEnrolled", true);
		
		//Subjects is an array of subject objects. Each subject object has the following properties: subjectName
		JSONArray subjects = new JSONArray();
		
		//Subject objects that are to be placed in the subjects array
		JSONObject ds = new JSONObject();
		ds.put("name", "Distributed Systems");
		JSONObject kt = new JSONObject();
		kt.put("name", "Knowledge Technologies");
		
		//Add the subject objects to the array
		subjects.add(ds);
		subjects.add(kt);
		
		//Add the subjects array to the student object
		student.put("subjects", subjects);
		
		//The toString() method in the JSONObject class automatically converts (marshals) a 
		//JSONObject to JSON formatted text
		//The toJSONString() method can be used as well
		System.out.println(student);
		
		//Store the JSON text in a file
		FileWriter file;
		try {
			file = new FileWriter("out.json");
			//Marshal the object
			String JSONText = student.toJSONString();
			file.write(JSONText);
			//file.flush();
			System.out.println("JSON file created");
			file.close();

		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	
	
	

}
