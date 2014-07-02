<?php
/**
 * @file
 * A module that provides commonly used functions throughout MoS.org
 */

function csv_to_array($csv, $delimiter = ',', $enclosure = '"', $escape = '\\', $terminator = "\n") { 
    $r = array(); 
    $rows = explode($terminator,trim($csv)); 
    $names = array_shift($rows); 
    $names = str_getcsv($names,$delimiter,$enclosure,$escape); 
    $nc = count($names); 
    foreach ($rows as $row) { 
        if (trim($row)) { 
            $values = str_getcsv($row,$delimiter,$enclosure,$escape); 
            if (!$values) $values = array_fill(0,$nc,null); 
            $r[] = array_combine($names,$values); 
        } 
    } 
    return $r; 
}

function array_to_csv($results){
    $output = array();
    foreach ($results[0] as $key=>$value ) {
    	if ($key != "CreatedDate"){
       		$headers[]=$key;
       	}
    }
    $header_row = implode(",",$headers);
    $output[0] = $header_row;
	foreach ( $results as $key=>$value ) {
		$answers = array();
		foreach($value as $k => $v){
			if ($k != "CreatedDate"){
	      	 	$answers[]=$v;
	      	}
	   	}
		$row[] = implode(",",$answers);  	
    }
   
    $all_rows = implode("\n", $row);
    $output[1] = $all_rows;
    //var_dump($output);
    $output = implode("\n", $output);
    return $output;
}

function load_block($block_id){
	$block = block_load('views', $block_id);
	$content = _block_get_renderable_array(_block_render_blocks(array($block)));
	$output = drupal_render($content);
	return $output;
}


function get_field_value($node, $field_name){
	$value = array();
	$field = field_get_items('node', $node, $field_name);
	if(!empty($field)){
		if (count($field)>1){
			foreach($field as $field_key => $field_value){
				$value[$field_key] = $field_value['value'];
			}
		}else{
			if (isset($field[0]["taxonomy_term"]->name)){
				return $field[0]["taxonomy_term"]->name;
			}else{
				return $field[0]['value'];
			}
		}
	}
	if(empty($value)){
		return false;
	}
	return $value;
}


function get_field_collection_value($node, $field_collection_name) {
	$value_array = array();
	$field_collection = field_get_items('node', $node, $field_collection_name);
	if (!empty($field_collection)){
		foreach ($field_collection as $field_collection_key => $field_collection_value){
	   		$field = field_view_value('node',$node, $field_collection_name, $field_collection_value); 
			$field_name = array(); 
			
			// Parse array to pull out the field labels of the fields within the field collection
			$field_collection_item_index = $field_collection_key +1; // the index for where the field names are located is the $field_collection_key plus one	
		    foreach ($field['entity']['field_collection_item'] as $id => $field_collection){
				foreach($field_collection as $key => $value){
					if (!preg_match("/\#/", $key)){
						$field_name[] = $key;
					}	
				}
		        // load the field collection item entity
		        $field_collection_item = field_collection_item_load($id);
		        // wrap the entity and make it easier to get the values of fields
		        $field_wrapper = entity_metadata_wrapper('field_collection_item', $field_collection_item);
				// store values in array which will be returned at end of function
				foreach ($field_name as $index => $name){
					$value_array[$field_collection_key][$name] = $field_wrapper->$name->value();
				}
				
		    }
			
		}
			
	}
	return $value_array;
}

function write_to_location($write_object, $write_location){
	$location_pointer = fopen ($write_location, "w");
	fwrite ($location_pointer, $write_object);
	fclose ($location_pointer);
}
