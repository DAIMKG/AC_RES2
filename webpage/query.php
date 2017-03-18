<!DOCTYPE html>
<html>
<head>
	<title>Result</title>
</head>
<body>
<!-- Display the input -->
<h3>Input:</h3>
<pre id="userInput">
<?php echo $_POST["input"]; ?>
</pre>

<hr />

<!-- Deal with the input and return matched results -->
<?php

$inputEntityFile = fopen("/home/wangzc/apache/tmp/inputEntity.txt", "w");

// split the input into groups
$group = explode("\r\n", $_POST["input"]);

// n is the number of groups
$n = count($group);

for ($i = 0; $i < $n; $i++) {
	$entities = explode(" ", $group[$i]);
	$m = count($entities);
	for ($j = 0; $j < $m; $j++) {
		fwrite($inputEntityFile, $entities[$j]);
		fwrite($inputEntityFile, "\n");
	}
}

fclose($inputEntityFile);




// execute findEntity.cpp

// exec("g++ /home/wangzc/apache/cpp_code/findEntity.cpp -o /home/wangzc/apache/cpp_code/findEntity -O3");
exec("/home/wangzc/apache/cpp_code/findEntity");




//display cancidateEntity.txt & create input.txt
$candidateEntityFile = fopen("/home/wangzc/apache/tmp/candidateEntity.txt", "r") or die("Unable to open file!");
$inputFile = fopen("/home/wangzc/apache/tmp/input.txt", "w");
print("<h3>Candidate entities:</h3>\n");
print("<form>\n");
for ($i = 0; $i < $n; $i++) {
	$entities = explode(" ", $group[$i]);
	$m = count($entities);
	fwrite($inputFile, $m);
	for ($j = 0; $j < $m; $j++) {
		print("<b>".$entities[$j]."</b>:<br />");
		for ($k = 0; $k < 5; $k++) { 
			$str = fgets($candidateEntityFile);
			print("<input type=\"radio\" name=\"".$i."_".$j."\" "."value=\"".substr($str, 1, -2)."\" ");
			if($k == 0) {
				fwrite($inputFile, " ".substr($str, 0, -1));
				print("checked />");
			}
			else print("/>");
			print(substr($str, 1, -2));
			print("<br />\n");
		}
		print("<hr />");
	}
	fwrite($inputFile, "\n");
}
print("</form>\n");

fclose($candidateEntityFile);
fclose($inputFile);


// execute find_type_sparql.cpp
exec("/home/wangzc/apache/cpp_code/find_type_sparql /home/wangzc/apache/tmp/input.txt /home/wangzc/apache/tmp/type_sparql.txt");

// run type_sparql.txt
exec("java -jar /home/wangzc/apache/cpp_code/QueryRDF_csv.jar /home/wangzc/apache/tmp/type_sparql.txt");

// run parse_csv_type.py
exec("python /home/wangzc/apache/cpp_code/parse_csv_type.py /home/wangzc/apache/tmp/type_sparql.txt_out.csv /home/wangzc/apache/tmp/type_sparql.txt_out.csv.tmp /home/wangzc/apache/tmp/common_types.txt");

// execute find_fact_sparql.cpp
exec("/home/wangzc/apache/cpp_code/find_fact_sparql /home/wangzc/apache/tmp/input.txt /home/wangzc/apache/tmp/fact_sparql.txt");

// run fact_sparql.txt
exec("java -jar /home/wangzc/apache/cpp_code/QueryRDF_csv.jar /home/wangzc/apache/tmp/fact_sparql.txt");

// run parse_fact_csv.py
exec("python /home/wangzc/apache/cpp_code/parse_fact_csv.py /home/wangzc/apache/tmp/fact_sparql.txt_out.csv /home/wangzc/apache/tmp/fact_sparql.txt_out.csv.tmp");

// execute find_fact_frequency.cpp
exec("/home/wangzc/apache/cpp_code/find_fact_frequency /home/wangzc/apache/tmp/fact_sparql.txt_out.csv.tmp /home/wangzc/apache/tmp/input.txt /home/wangzc/apache/tmp/sparql_find_fact_f /home/wangzc/apache/tmp/common_facts.txt");

// run parse_again.py
exec("python /home/wangzc/apache/cpp_code/parse_facts_again.py /home/wangzc/apache/tmp/common_facts.txt /home/wangzc/apache/tmp/common_facts_parsed.txt");

// run sparql_find_fact_f
exec("java -jar /home/wangzc/apache/cpp_code/QueryRDF_csv.jar /home/wangzc/apache/tmp/sparql_find_fact_f");

// run attributes.py
exec("python /home/wangzc/apache/cpp_code/rank_attributes.py /home/wangzc/apache/tmp/common_facts_parsed.txt /home/wangzc/apache/tmp/sparql_find_fact_f_out.csv /home/wangzc/apache/tmp/ranked_facts_using_sparql");

// execute findPattern.cpp
exec("/home/wangzc/apache/cpp_code/findPattern");

// maximal_common_substructure.cpp
exec("/home/wangzc/apache/cpp_code/maximal_common_substructure /home/wangzc/apache/tmp/pattern.txt /home/wangzc/apache/tmp/common_pattern.txt");

// naive_sparql_forming.cpp
exec("/home/wangzc/apache/cpp_code/naive_sparql_forming /home/wangzc/apache/tmp/input.txt /home/wangzc/apache/tmp/common_pattern.txt /home/wangzc/apache/tmp/ranked_facts_using_sparql /home/wangzc/apache/tmp/common_types.txt /home/wangzc/apache/tmp/pattern.txt 2 1");

// display sparql file

$finalSparqlFile = fopen("/home/wangzc/apache/tmp/sparql_2_1.txt", "r") or die("Unable to open file!");

print("<h3>SPARQL:</h3>\n");
print("<pre>\n");
print(htmlspecialchars(fread($finalSparqlFile, filesize("/home/wangzc/apache/tmp/sparql_2_1.txt"))));
print("</pre>\n");

fclose($finalSparqlFile);

print("<hr />\n");

// exeute final_sparql
exec("java -jar /home/wangzc/apache/cpp_code/QueryRDF_csv.jar /home/wangzc/apache/tmp/sparql_2_1.txt");



// display result
$result = fopen("/home/wangzc/apache/tmp/sparql_2_1.txt_out.csv", "r") or die("Unable to open file!");

print("<h3>Result:</h3>\n");
print("<pre>\n");
print(htmlspecialchars(fread($result, filesize("/home/wangzc/apache/tmp/sparql_2_1.txt_out.csv"))));
print("</pre>\n");

fclose($result);

?>


<hr />
<a href="/index.html">Return</a>


</body>

</html>




