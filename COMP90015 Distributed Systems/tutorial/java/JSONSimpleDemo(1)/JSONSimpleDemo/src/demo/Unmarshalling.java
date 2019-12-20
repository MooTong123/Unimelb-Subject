package demo;

import java.io.FileReader;
import java.io.Reader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

//Read a JSON formatted text stored in a file, unmarshal it to create a JSONObject, use the JSONObject 
//to access the value/name pairs of the JSON text.
public class Unmarshalling {

	public static void main(String[] args) {
		
		//Parser used to convert (unmarshal) a string into a JSONObject
		JSONParser parser = new JSONParser();

		try {
		
			//Read the file and unmarshal the contents
			Reader in = new FileReader("out.json");
			JSONObject studentJsonObj = (JSONObject) parser.parse(in);
			in.close();
			
			//Obtain the student properties from the JSONObject
			Number id = (Number) studentJsonObj.get("ID");
			System.out.println("ID: " + id);
			String name = (String) studentJsonObj.get("name");
			System.out.println("Name:" + name);
			Boolean isEnrolled = (Boolean) studentJsonObj.get("isEnrolled");
			System.out.println("Is enrolled: " + isEnrolled);

			//Access the subjects array
			JSONArray subjects = (JSONArray) studentJsonObj.get("subjects");
			Iterator<JSONObject> it = subjects.iterator();
			List<String> subjectsList = new ArrayList<>(); //Used to store the subject names
			System.out.println("Subjects:");
			while (it.hasNext()) {
				JSONObject subject = it.next();
				String subName = (String) subject.get("name");
				System.out.println(subName);
				subjectsList.add(subName);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
