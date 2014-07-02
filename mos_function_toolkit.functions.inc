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

/**
* Converts CSV to PHP Array
*
* @csv  String, CSV string that needs to be turned into a php Array
*
* @r    Array, an array version of the csv that was inputted into this function
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

/**
* Converts PHP Array into CSV String
*
* @results  Array, PHP array that needs to be turned into a csv string
*
* @output   String, a csv string version of the php array that was inputted into this function
*/
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

/**
* Loads a drupal block in a single function call
*
* @blockId  String, drupal block ID
*
* @output   Object, a drupal rendered object that can be printed in a tpl.php file
*/
function loadBlock($blockId)
{
    $block = block_load('views', $blockId);
    $content = _block_get_renderable_array(_block_render_blocks(array($block)));
    $output = drupal_render($content);
    return $output;
}

/**
* Loads a drupal field in a single function call, this can also handle taxonomy terms
* Please note that this only works for single input fields
*
* @node  Object, the current node that is being loaded
* @fieldName  String, drupal field machine name
*
* @value   String, The value of the field
*/
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

/**
* Loads a drupal field collection in a single function call
*
* @node  Object, the current node that is being loaded
* @fieldCollectionName  String, drupal field collection machine name
*
* @value_array   Array, The value of the field collection. This will 
                return value in the order that they are entered in Drupal Admin
*/
function getFieldCollectionValue($node, $fieldCollectionName) 
{
    $valueArray = array();
    $fieldCollection = field_get_items('node', $node, $fieldCollectionName);
    if (!empty($fieldCollection)){
        foreach ($fieldCollection as $fieldCollectionKey => $fieldCollectionValue){
            $field = field_view_value('node',$node, $fieldCollectionName, $fieldCollectionValue); 
            $fieldName = array(); 
            
            // Parse array to pull out the field labels of the fields within the field collection
            $fieldCollectionItemIndex = $fieldCollectionKey +1;             
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

/**
* Writes file to a location a single function call
*
* @writeObject  Multi Data Type, the thing that will be written to a file
* @writeLocation  String, the path of the file
*
* No Return Value
*/
function writeToLocation($writeObject, $writeLocation)
{
    $locationPointer = fopen ($writeLocation, "w");
    fwrite ($locationPointer, $writeObject);
    fclose ($locationPointer);
}