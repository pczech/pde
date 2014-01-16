<?php

/**
 * @file
 * Common tools for PDE modules.
 */


/**
 * Traces function calls.
 */
function pde_trace_funct() {
  $trace = debug_backtrace();
  if (isset($trace[1])) {
    dpm($trace[1]['function']);
  }
}


function pde_dpm($data, $info) {
  dpm($info . ':');
  dpm($data);
}
