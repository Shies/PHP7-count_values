<?php
/*
class Demo
{
    public $target = 'test.txt';
    public $__ = ':';
    private $source;

    function __construct()
    {
        $this->source = @file_get_contents($this->target);
    }

    function output()
    {
        static $_items = array();

        foreach (explode(PHP_EOL, $this->source) AS $val)
        {
            $this->ItemHandle($_items, $val);
            unset($val);
        }

        return $_items;
    }

    function handle(&$items, &$val)
    {
        list($head, $tail) = explode($this->__, $val);
        if (isset($items[$head]))
        {
            $items[$head] += $tail;
        }
        else
        {
            $items[$head] = $tail;
        }
    }

    function valid()
    {
        return NULL !== $this->source;
    }
}
*/

$demo = new demo;
var_dump($demo->output());
