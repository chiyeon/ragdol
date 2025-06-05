pipeline {
   agent any

   environment {
      BUILD_DIR = 'build'
   }

   stages {
      stage('Preperation') {
         steps {
            echo 'Preparing workspace'
            sh 'rm -rf $BUILD_DIR'
            sh 'cmake -S . -B $BUILD_DIR'
         }
      }

      stage('Linting') {
         steps {
            echo 'Running format checks'
            sh 'cmake --build build --target check-format'
         }
      }

      stage('Build') {
         steps {
            echo 'Building ragdol project'
            sh 'cmake --build build'
         }
      }

      stage('Test') {
         steps {
            echo 'Running unit tests'
            echo 'WARNING: unit tests not set up'
            sh './build/ragdol ./test/fib.rd'
         }
      }

   }
}
