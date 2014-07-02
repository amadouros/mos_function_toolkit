<?php
/**
 * Functions for a module that provides commonly used functionality throughout MoS.org
 *
 * @project         mos_function_toolkit
 * @version         0.1
 * @author          Arika Prime
 * @lastEdited      07/02/2014
 * @lastEditedBy    Arika Prime
 */




function csvToArray($csv, $delimiter = ',', $enclosure = '"', $escape = '\\', $terminator = "\n") 
{ 
    $r = array(); 
    $rows = explode($terminator,trim($csv)); 
    $names = array_shift($rows); 
    $names = str_getcsv($names, $delimiter, $enclosure, $escape); 
    $nc = count($names); 

    //convert each row into array element, with header values as keys
    foreach ($rows as $row) { 
        if (trim($row)) { 
            $values = str_getcsv($row, $delimiter, $enclosure, $escape); 
            if (!$values) $values = array_fill(0, $nc, null); 
            $r[] = array_combine($names,$values); 
        } 
    } 
    return $r; 
}

function arrayToCsv($results)
{
    $output = array();
    //Get header row values from first element of array, seperated by columns
    foreach ($results[0] as $key=>$value ) {
        $headers[]=$key;
    }
    $headerRow = implode(",",$headers);
    $output[0] = $headerRow;

    //Get each row of values from entire array, seperated by columns
    foreach ( $results as $key=>$value ) {
        $answers = array();
        foreach($value as $k => $v){
            $answers[]=$v;
        }
        $row[] = implode(",", $answers);  	
    }

    //Implode all rows, seperated by columns
    $allRows = implode("\n", $row);
    $output[1] = $allRows;
    $output = implode("\n", $output);
    return $output;
}

function loadBlock($blockId)
{
    $block = block_load('views', $blockId);
    $content = _block_get_renderable_array(_block_render_blocks(array($block)));
    $output = drupal_render($content);
    return $output;
}

function getFieldValue($node, $fieldName)
{
    $value = array();
    $field = field_get_items('node', $node, $fieldName);
    if(!empty($field)){
        if (count($field)>1){
            foreach($field as $fieldKey => $fieldValue){
                $value[$fieldKey] = $fieldValue['value'];
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


function get_field_collection_value($node, $field_collection_name) 
{
    $valueArray = array();
    $fieldCollection = field_get_items('node', $node, $fieldCollectionName);
    if (!empty($fieldCollection)){
        foreach ($fieldCollection as $fieldCollectionKey => $fieldCollectionValue){
            $field = field_view_value('node',$node, $fieldCollectionName, $fieldCollectionValue); 
            $fieldName = array(); 
            
            // Parse array to pull out the field labels of the fields within the field collection
            $fieldCollectionItemIndex = $fieldCollectionKey +1; // the index for where the field names are located is the $fieldCollectionKey plus one	
            foreach ($field['entity']['field_collection_item'] as $id => $fieldCollection){
                foreach($fieldCollection as $key => $value){
                    if (!preg_match("/\#/", $key)){
                        $fieldName[] = $key;
                    }
                }
                // load the field collection item entity
                $fieldCollectionItem = field_collection_item_load($id);
                // wrap the entity and make it easier to get the values of fields
                $fieldWrapper = entity_metadata_wrapper('field_collection_item', $fieldCollectionItem);
                // store values in array which will be returned at end of function
                foreach ($fieldName as $index => $name){
                    $valueArray[$fieldCollectionKey][$name] = $fieldWrapper->$name->value();
                }
            }
        }
    }
    return $value_array;
}

function writeToLocation($writeObject, $writeLocation)
{
    $locationPointer = fopen ($writeLocation, "w");
    fwrite ($locationPointer, $writeObject);
    fclose ($locationPointer);
}